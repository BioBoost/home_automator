#pragma once

#include "MQTTThreadedClient.h"
#include "mqtt_message.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  class MQTTChannel {

    public:
      MQTTChannel(NetworkInterface* network, std::string serverAddress, std::string clientId, unsigned int port=1883);
      virtual ~MQTTChannel(void);

    public:
      void publish(std::string topic, std::string message);
      void publish(MQTTMessage message);

    template<typename T>
    void subscribe(std::string topic, T *object, void (T::*member)(MQTT::MessageData &)) {
#ifdef DO_SIMPLE_LOG
        Log.verbose("Subscribing to topic " + topic);
#endif
        mqttClient.addTopicHandler(topic.c_str(), object, member);
    }

    template<typename T>
    void set_default_topic_handler(T *object, void (T::*member)(MQTT::MessageData &)) {
#ifdef DO_SIMPLE_LOG
        Log.verbose("Setting default topic handler");
#endif
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
  };

};
