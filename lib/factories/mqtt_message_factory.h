#pragma once

#include <string>
#include "mqtt_message.h"
#include "input.h"

namespace BiosHomeAutomator {

  class MQTTMessageFactory {

    public:
      static MQTTMessage * create_input_state_update(Input * input) {
        std::string topic = MQTT_CARD_TOPIC_BASE + std::to_string(input->get_parent()->get_id())
          + "/inputs/" + std::to_string(input->get_id());

        std::string payload = "";
        if (input->get_time_between_last_events() > SHORT_PRESS_TIME_MS && input->get_time_between_last_events() <= MAX_LONG_PRESS_TIME_MS) {
          payload = "LONG_PRESS";
        } else if (input->get_time_between_last_events() <= SHORT_PRESS_TIME_MS) {
          payload = "SHORT_PRESS";
        } else {
          return nullptr;
        }

        MQTTMessage * message = new MQTTMessage(topic, payload);
        return message;
      }

  };

};
