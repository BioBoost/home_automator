#pragma once

#include "MQTTThreadedClient.h"

namespace BiosHomeAutomator {

  class MQTTChannel {

    public:
      MQTTChannel(NetworkInterface* network, std::string serverAddress, std::string clientId, unsigned int port=1883);
      virtual ~MQTTChannel(void);

    public:
      void publish(std::string topic, std::string message);

    private:
      void init_mqtt_connection(std::string serverAddress, unsigned int port);
      void start_processing_thread(void);
      void stop_processing_thread(void);

    private:
      MQTT::MQTTThreadedClient mqttClient;
      char clientId[100];
      Thread mqttProcessingThread;
      int messageId;
  };

};
