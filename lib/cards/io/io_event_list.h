#pragma once

#include "io_event.h"

namespace BiosHomeAutomator {

  class IOEventList {

    public:
      void add(IOEvent event);
      uint64_t get_time_between_last_events(void);
      IOEvent get_last_event(void);

    private:
      static const unsigned int SIZE_OF_LIST = 2;    
      IOEvent pastEvents[SIZE_OF_LIST];

  };

};