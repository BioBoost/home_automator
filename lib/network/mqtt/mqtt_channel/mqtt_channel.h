#pragma once

#include "MQTTThreadedClient.h"
#include "mqtt_message.h"
#include "simple_logger.h"

namespace BiosHomeAutomator {

  class ConnectionHandler {
     public:
      void connected_handler(void) {
        info("MQTT connected - inside handler");
      }
  };

  class MQTTChannel {

    public:
      MQTTChannel(NetworkInterface* network, std::string serverAddress, std::string clientId, unsigned int port=1883);
      virtual ~MQTTChannel(void);

    public:
      void publish(std::string topic, std::string message);
      void publish(MQTTMessage message);

    template<typename T>
    void subscribe(std::string topic, T *object, void (T::*member)(MQTT::MessageData &)) {
        verbose("Subscribing to topic " + topic);
        mqttClient.addTopicHandler(topic.c_str(), object, member);
    }

    template<typename T>
    void set_default_topic_handler(T *object, void (T::*member)(MQTT::MessageData &)) {
        verbose("Setting default topic handler");
        mqttClient.setDefaultTopicHandler(object, member);
    }

    private:
      void init_mqtt_connection(std::string serverAddress, unsigned int port);
      void start_processing_thread(void);
      void stop_processing_thread(void);

    private:
      MQTT::MQTTThreadedClient mqttClient;
      char clientId[100];
      Thread mqttProcessingThread;
      int messageId;
      ConnectionHandler connectionHandler;
  };

};
