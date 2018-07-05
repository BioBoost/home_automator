#include "mqtt_channel.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  MQTTChannel::MQTTChannel(EthernetInterface * ethernet, std::string broker, std::string clientId, unsigned int port)
    : mqttNetwork(ethernet) {

    client = new MQTT::Client<MQTTNetwork, Countdown>(mqttNetwork);
 
    if (mqttNetwork.connect(broker.c_str(), port)) {
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
    mqttNetwork.disconnect();
    delete client;
  }

  void MQTTChannel::publish(std::string message, std::string topic) {
    publish(MQTTMessage(message, topic));
  }

  void MQTTChannel::publish(MQTTMessage message) {
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
      osEvent event = publishMailbox.get();
      if (event.status == osEventMail) {
        MQTTMessage * queueMessage = (MQTTMessage*)event.value.p;
#ifdef DO_SIMPLE_LOG
        Log.verbose("Dequeuing message " + queueMessage->to_string());
#endif
        publishMailbox.free(queueMessage);
      }
    }
#ifdef DO_SIMPLE_LOG
    Log.info("Publisher thread finished");
#endif
  }
};
