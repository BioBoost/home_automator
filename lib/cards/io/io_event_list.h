#pragma once

#include "io_event.h"

namespace BiosHomeAutomator {

  class IOEventList {

    public:
      void add(IOEvent event);

    private:
      static const unsigned int SIZE_OF_LIST = 2;    
      IOEvent pastEvents[SIZE_OF_LIST];

  };

};