#include "input.h"

namespace BiosHomeAutomator {

  Input::Input(ExpansionCard * parent, unsigned int id, State state) {
    this->parent = parent;
    this->id = id;
    this->state = state;
  }

  ExpansionCard * Input::get_parent(void) {
    return parent;
  }

  unsigned int Input::get_id(void) {
    return id;
  }

  void Input::set_state(State newState) {
    if (state == newState) {
      return;
    }

    IOEvent event(determine_edge(state, newState));
    pastEvents.add(event);
    state = newState;
  }

  IOEvent::Edge Input::determine_edge(State previous, State current) {
    if (previous == current) {
      return IOEvent::NONE;
    } else if (current == HIGH) {
      return IOEvent::RISING;
    } else {
      return IOEvent::FALLING;
    }
  }

  uint64_t Input::get_time_between_last_events(void) {
    return pastEvents.get_time_between_last_events();
  }

  std::string Input::to_string(void) {
    std::string result = "Input " + std::to_string(id) + " is " + std::string(state == LOW ? "LOW" : "HIGH");
    result += " => Last Event = " + pastEvents.get_last_event().to_string();
    return result;
  }

};