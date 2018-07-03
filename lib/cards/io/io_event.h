#pragma once

#include "mbed.h"
#include <string>

namespace BiosHomeAutomator {

  class IOEvent {

    public:

      enum Edge {
        RISING, FALLING
      };

    public:
      IOEvent(Edge edge);
      IOEvent(Edge edge, uint64_t timeSinceBoot);
      virtual ~IOEvent(void);

    public:
      std::string to_string(void);

    private:
      Edge edge;
      uint64_t timeSinceBoot;

  };

};