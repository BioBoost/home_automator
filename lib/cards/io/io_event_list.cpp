#include "io_event_list.h"

namespace BiosHomeAutomator {

  void IOEventList::add(IOEvent event) {
    for (unsigned int i = SIZE_OF_LIST-1; i > 0; i--) {
      pastEvents[i] = pastEvents[i-1];
    }
    pastEvents[0] = event;
  }

  uint64_t IOEventList::get_time_between_last_events(void) {
    return (pastEvents[0].get_time() - pastEvents[1].get_time());
  }

  IOEvent IOEventList::get_last_event(void) {
    return pastEvents[0];
  }

};