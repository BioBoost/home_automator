#include "io_event.h"
#include "time_since_boot.h"

namespace BiosHomeAutomator {

  IOEvent::IOEvent(Edge edge) {
    this->edge = edge;
    timeSinceBoot = BootTimer.millis_since_boot();
  }

  IOEvent::IOEvent(Edge edge, uint64_t timeSinceBoot)
    : IOEvent(edge) {
    this->timeSinceBoot = timeSinceBoot;
  }

  IOEvent::~IOEvent(void) { }

  std::string IOEvent::to_string(void) {
    std::string result = "IOEvent: " + std::string(edge == RISING ? "RISING" : "FALLING") + " edge";
    result += " @" + std::to_string(timeSinceBoot) + "ms since boot";
    return result;
  }

};