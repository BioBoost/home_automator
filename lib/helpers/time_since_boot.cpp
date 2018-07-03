#include "time_since_boot.h"

namespace Helpers {
  
  void TimeSinceBoot::init(void) {
    // Does nothing but call to get_instance() will initialize
    // the instance
  }

  TimeSinceBoot& TimeSinceBoot::get_instance() {
      static TimeSinceBoot instance;   // Guaranteed to be destroyed.
      return instance;
  }

  TimeSinceBoot::TimeSinceBoot(void) {
    millisSinceBoot = 0;
    resetTicker.attach(callback(this, &TimeSinceBoot::overflow_protection), 600);
    timer.start();
  }
  
  TimeSinceBoot::~TimeSinceBoot(void) { }

  uint64_t TimeSinceBoot::millis_since_boot(void) {
    return (millisSinceBoot + timer.read_ms());
  }

  void TimeSinceBoot::overflow_protection(void) {
    millisSinceBoot += timer.read_ms();
    timer.reset();
  }

};

