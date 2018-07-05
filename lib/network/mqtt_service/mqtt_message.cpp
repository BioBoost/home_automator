#include "mqtt_message.h"

namespace BiosHomeAutomator {

  unsigned int MQTTMessage::i = 0;

  MQTTMessage::MQTTMessage(void)
    : MQTTMessage("", "") {
  }

  MQTTMessage::MQTTMessage(std::string message, std::string topic) {
    set_message(message);
    set_topic(topic);
  }

  MQTTMessage::~MQTTMessage(void) { }

  std::string MQTTMessage::get_topic(void) {
    return topic;
  }

  std::string MQTTMessage::get_message(void) {
    return message;
  }

  void MQTTMessage::set_message(std::string message) {
    this->message = message;
  }

  void MQTTMessage::set_topic(std::string topic) {
    this->topic = topic;
  }

  std::string MQTTMessage::to_string(void) {
    return message + " @ " + topic;
  }

};
