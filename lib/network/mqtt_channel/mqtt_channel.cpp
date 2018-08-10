#include "mqtt_channel.h"
// #include <cstdio>
// #include <cstring>
#include "mbed.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  MQTTChannel::MQTTChannel(NetworkInterface* network, std::string serverAddress, std::string clientId, unsigned int port) 
    : mqttClient(network), mqttProcessingThread(osPriorityNormal, MQTT_PROCESS_THREAD_STACK_SIZE) {

      sprintf(this->clientId, clientId.c_str());
      this->messageId = 0;
      init_mqtt_connection(serverAddress, port);
      start_processing_thread();
  }

  MQTTChannel::~MQTTChannel(void) {
    stop_processing_thread();
  }

  void MQTTChannel::init_mqtt_connection(std::string serverAddress, unsigned int port) {
      MQTTPacket_connectData logindata = MQTTPacket_connectData_initializer;
      logindata.MQTTVersion = 3;
      logindata.clientID.cstring = this->clientId;
      mqttClient.setConnectionParameters(serverAddress.c_str(), port, logindata);
  }

  void MQTTChannel::start_processing_thread(void) {
    #ifdef DO_SIMPLE_LOG
        Log.info("Starting publisher thread");
    #endif
    mqttProcessingThread.start(mbed::callback(&mqttClient, &MQTT::MQTTThreadedClient::startListener));
  }

  void MQTTChannel::stop_processing_thread(void) {
    mqttProcessingThread.join();   // Wait for thread to finish
  }

  void MQTTChannel::publish(std::string topic, std::string message) {
    MQTT::PubMessage messageToPublish;
    messageToPublish.qos = MQTT::QOS0;
    messageToPublish.id = messageId++;
    strcpy(&messageToPublish.topic[0], topic.c_str());
    strcpy(&messageToPublish.payload[0], message.c_str());
    messageToPublish.payloadlen = message.length();
    mqttClient.publish(messageToPublish);
#ifdef DO_SIMPLE_LOG
    Log.debug("Publishing message " + message + " @ " + topic);
#endif
  }

};
