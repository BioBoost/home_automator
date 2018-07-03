#pragma once

#include "mbed.h"

#define BootTimer Helpers::TimeSinceBoot::get_instance()

namespace Helpers {

  class TimeSinceBoot final {

    public:
      void init(void);    // Must be called on system boot to initialize instance

    public:
      static TimeSinceBoot& get_instance();

    private:
        TimeSinceBoot(void);   // Make constructor private
        ~TimeSinceBoot(void);

    public:
      uint64_t millis_since_boot(void);

    private:
      void overflow_protection(void);

    // Dont forget to declare these two. You want to make sure they
    // are unacceptable otherwise you may accidentally get copies of
    // your singleton appearing.
    public:
        TimeSinceBoot(TimeSinceBoot const&)   = delete;
        void operator=(TimeSinceBoot const&)  = delete;
        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status

    private:
      Timer timer;
      Ticker resetTicker;
      uint64_t millisSinceBoot;
  };

};
