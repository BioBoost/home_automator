#include "mqtt_channel.h"
#include "mbed.h"

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
    info("Starting publisher thread");
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
    debug("Publishing '" + message + "' @ " + topic);
  }

  void MQTTChannel::publish(MQTTMessage message) {
    publish(message.get_topic(), message.get_message());
  }

};
