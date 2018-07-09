#include "mqtt_channel.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  MQTTChannel::MQTTChannel(EthernetInterface * ethernet, std::string broker, std::string clientId, unsigned int port)
    : mqttSocket(ethernet) {

    client = new MQTT::Client<MQTTSocket, Countdown>(mqttSocket);
 
    if (mqttSocket.connect((char*)broker.c_str(), port)) {
#ifdef DO_SIMPLE_LOG
    Log.warning("Connection to MQTT broker failed");
#endif
    } else {
#ifdef DO_SIMPLE_LOG
    Log.info("Connected to MQTT broker");
#endif
    }

    MQTTPacket_connectData connectionOptions = MQTTPacket_connectData_initializer;
    connectionOptions.MQTTVersion = 3;
    connectionOptions.clientID.cstring = (char*)(clientId.c_str());
    if (client->connect(connectionOptions)) {
#ifdef DO_SIMPLE_LOG
      Log.warning("MQTT Client connection failed");
#endif
    }

    keepPublishing = false;
    start_publishing_thread();
  }

  MQTTChannel::~MQTTChannel(void) {
    stop_publishing_thread();
    client->disconnect();
    mqttSocket.disconnect();
    delete client;
  }

  void MQTTChannel::publish(std::string message, std::string topic) {
    publish(MQTTMessage(message, topic));
  }

  void MQTTChannel::publish(MQTTMessage message) {
    if (publishMailbox.full()) {
#ifdef DO_SIMPLE_LOG
      Log.warning("Mailbox for publish messages is full.");
#endif
    }

    MQTTMessage * mqttMessage = publishMailbox.alloc();
    (*mqttMessage) = message;
#ifdef DO_SIMPLE_LOG
    Log.verbose("Enqueuing message " + mqttMessage->to_string());
#endif
    publishMailbox.put(mqttMessage);
  }

  void MQTTChannel::start_publishing_thread(void) {
    keepPublishing = true;
    publisherThread.start(callback(this, &MQTTChannel::publishing_thread));
  }

  void MQTTChannel::stop_publishing_thread(void) {
    keepPublishing = false;
    publisherThread.join();   // Wait for thread to finish
  }

  void MQTTChannel::publishing_thread(void) {
#ifdef DO_SIMPLE_LOG
    Log.info("Starting publisher thread");
#endif
    while(keepPublishing) {
      // Try to dequeue message
      osEvent event = publishMailbox.get(MS_TO_WAIT_FOR_MAILBOX);
      if (event.status == osEventMail) {
        MQTTMessage * queueMessage = (MQTTMessage*)event.value.p;
#ifdef DO_SIMPLE_LOG
        Log.verbose("Dequeuing message " + queueMessage->to_string());
#endif
        publishMailbox.free(queueMessage);
      }
      if (client->yield(100)) {
#ifdef DO_SIMPLE_LOG
        Log.warning("No connection to MQTT broker to yield()");
#endif
      }
    }
#ifdef DO_SIMPLE_LOG
    Log.info("Publisher thread finished");
#endif
  }
};
