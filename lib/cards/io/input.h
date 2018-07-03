#pragma once

#include <string>
#include "expansion_card.h"
#include "io_event_list.h"

namespace BiosHomeAutomator {

  class Input {

    public: enum State { HIGH, LOW };

    public:
      Input(ExpansionCard * parent, unsigned int id, State state);

    public:
      ExpansionCard * get_parent(void);
      unsigned int get_id(void);

    public:
      void set_state(State newState);
      uint64_t get_time_between_last_events(void);

    public:
      std::string to_string(void);

    private:
      IOEvent::Edge determine_edge(State previous, State current);

    private:
      ExpansionCard * parent;
      unsigned int id;
      State state;
      IOEventList pastEvents;
  };

};