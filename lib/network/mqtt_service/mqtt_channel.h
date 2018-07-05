#pragma once

#include "mbed.h"
#include <string>
#include <EthernetInterface.h>
#include "mqtt_network.h"
#include "MQTTmbed.h"
#include "MQTTClient.h"
#include "mqtt_message.h"

namespace BiosHomeAutomator {

  class MQTTChannel {

    public:
      MQTTChannel(EthernetInterface * ethernet, std::string broker, std::string clientId, unsigned int port=1883);
      virtual ~MQTTChannel(void);

    public:
      void publish(std::string message, std::string topic);
      void publish(MQTTMessage message);

    private:
      void start_publishing_thread(void);
      void stop_publishing_thread(void);
      void publishing_thread(void);

    private:
      static const unsigned int SIZE_OF_PUBLISHER_MAIL = 8;
      static const unsigned int MS_TO_WAIT_FOR_MAILBOX = 10;
      static const MQTT::QoS DEFAULT_QOS = MQTT::QOS2;
      static const unsigned int MAX_MESSAGE_SIZE = 256;
      static const unsigned int MS_FOR_CLIENT_TO_YIELD = 50;
      MQTTNetwork mqttNetwork;
      MQTT::Client<MQTTNetwork, Countdown> * client;

      // Threading stuff
      Thread publisherThread;
      bool keepPublishing;
      Mail<MQTTMessage, SIZE_OF_PUBLISHER_MAIL> publishMailbox;
  };

};