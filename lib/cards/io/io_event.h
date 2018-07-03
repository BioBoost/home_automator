#pragma once

#include "mbed.h"
#include <string>

namespace BiosHomeAutomator {

  class IOEvent {

    public:

      enum Edge {
        NONE, RISING, FALLING
      };

    public:
      IOEvent(void);
      IOEvent(Edge edge);
      IOEvent(Edge edge, uint64_t timeSinceBoot);
      virtual ~IOEvent(void);

    public:
      uint64_t get_time(void);
      Edge get_edge(void);

    public:
      std::string to_string(void);

    private:
      std::string edge_to_string(Edge edge);

    private:
      Edge edge;
      uint64_t timeSinceBoot;

  };

};