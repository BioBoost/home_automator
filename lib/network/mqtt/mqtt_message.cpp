#include "mqtt_message.h"

namespace BiosHomeAutomator {

  MQTTMessage::MQTTMessage(std::string topic, std::string message) {
    this->topic = topic;
    this->message = message;
  }

  std::string MQTTMessage::get_topic(void) {
    return topic;
  }

  std::string MQTTMessage::get_message(void) {
    return message;
  }

};