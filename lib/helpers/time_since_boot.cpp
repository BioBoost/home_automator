#include "time_since_boot.h"

namespace Helpers {

  TimeSinceBoot::TimeSinceBoot(void) {
    millisSinceBoot = 0;
    resetTicker.attach(callback(this, &TimeSinceBoot::overflow_protection), 600);
    timer.start();
  }

  uint64_t TimeSinceBoot::millis_since_boot(void) {
    return (millisSinceBoot + timer.read_ms());
  }

  void TimeSinceBoot::overflow_protection(void) {
    millisSinceBoot += timer.read_ms();
    timer.reset();
  }

};