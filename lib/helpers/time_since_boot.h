#pragma once

#include "mbed.h"

namespace Helpers {

  class TimeSinceBoot {

    public:
      TimeSinceBoot(void);

    public:
      uint64_t millis_since_boot(void);

    private:
      void overflow_protection(void);

    private:
      Timer timer;
      Ticker resetTicker;
      uint64_t millisSinceBoot;
  };

};