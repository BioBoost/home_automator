#include "io_event_list.h"

namespace BiosHomeAutomator {

  void IOEventList::add(IOEvent event) {
    for (unsigned int i = SIZE_OF_LIST-1; i > 0; i--) {
      pastEvents[i] = pastEvents[i-1];
    }
    pastEvents[0] = event;
  }

};