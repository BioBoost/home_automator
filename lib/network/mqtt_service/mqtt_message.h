#pragma once

#include <string>

namespace BiosHomeAutomator {

  class MQTTMessage {

    public:
      MQTTMessage(void);
      MQTTMessage(std::string message, std::string topic);
      virtual ~MQTTMessage(void);

    public:
      std::string get_topic(void);
      std::string get_message(void);

    public:
      void set_message(std::string message);
      void set_topic(std::string topic);

    private:
      std::string topic;
      std::string message;
  };

};