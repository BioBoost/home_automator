#include "io_event.h"
#include "time_since_boot.h"

namespace BiosHomeAutomator {

  IOEvent::IOEvent(void)
    : IOEvent(NONE) {
  }

  IOEvent::IOEvent(Edge edge)
    : IOEvent(edge, BootTimer.millis_since_boot()) {
  }

  IOEvent::IOEvent(Edge edge, uint64_t timeSinceBoot) {
    this->edge = edge;
    this->timeSinceBoot = timeSinceBoot;
  }

  IOEvent::~IOEvent(void) { }

  uint64_t IOEvent::get_time(void) {
    return timeSinceBoot;
  }

  IOEvent::Edge IOEvent::get_edge(void) {
    return edge;
  }

  std::string IOEvent::to_string(void) {
    std::string result = "IOEvent: " + edge_to_string(edge) + " edge";
    result += " @" + std::to_string(timeSinceBoot) + "ms";
    return result;
  }

  std::string IOEvent::edge_to_string(Edge edge) {
    if (edge == RISING) {
      return "RISING";
    } else if (edge == FALLING) {
      return "FALLING";
    }
    return "NONE";
  }

};