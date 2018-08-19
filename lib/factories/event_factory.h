#pragma once

#include <string>
#include "mqtt_message.h"
#include "string_splitter.h"
#include "relay_event.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  class EventFactory {

    public:
      // Requires refactor. The checks are depending on a hardcoded topic format while
      // the topic is configurable via the library json config file.
      static CardEvent * create_event_from_mqtt_message(MQTTMessage mqttMessage) {
        std::string topic =  mqttMessage.get_topic();
        std::string payload = mqttMessage.get_message();
        std::vector<std::string> topicParts = Helpers::StringSplitter::split(topic, '/');
        // "home/cards/+/relays/+/set"

        CardEvent * event = nullptr;

        if (topicParts.size() == 6 && topicParts[3] == "relays" && topicParts[5] == "set") {
          unsigned int cardId = std::stoi(topicParts[2]);   // Should catch the exceptions
          unsigned int relayId = std::stoi(topicParts[4]);
          RelayEventType eventType;

          if (payload == "ON") {
            eventType = ON;
          } else if (payload == "OFF") {
            eventType = OFF;
          } else if (payload == "TOGGLE") {
            eventType = TOGGLE;
          } else {
#ifdef DO_SIMPLE_LOG
            Log.warning("Unknown event type in MQTT message: " + payload);
#endif
            return nullptr;
          }
          event = new RelayEvent(cardId, relayId, eventType);
        } else {
#ifdef DO_SIMPLE_LOG
          Log.warning("Missing event topic parts in MQTT message: " + topic);
#endif
        }

        return event;
      }

  };

};
