#include "io_relay_card.h"

namespace BiosHomeAutomator {

  IORelayCard::IORelayCard(unsigned int ioExpanderI2cAddress, I2C * i2c, PinName ioInterrupt, unsigned int id)
    : ExpansionCard(id), ioExpander(ioExpanderI2cAddress, i2c, ioInterrupt) {

    ioExpander.set_port_direction(EXPANDER_PORT_MASK);
    all_relays_off();

    initialize_inputs();
  }

  void IORelayCard::initialize_inputs(void) {
    unsigned int data = ioExpander.read();
    for (unsigned int i = 0; i < NUMBER_OF_INPUTS; i++) {
      Input * input = new Input(this, i, determine_input_state(data, i));
      inputs.push_back(input);
    }
  }

  Input::State IORelayCard::determine_input_state(unsigned int portState, unsigned int inputId) {
    if ((portState & (0x0001 << inputId)) != 0) {
      return Input::HIGH;
    } else {
      return Input::LOW;
    }
  }

  IORelayCard::~IORelayCard(void) {
    for (unsigned int i = 0; i < inputs.size(); i++) {
      delete inputs[i];
    }
    inputs.clear();
  }

  void IORelayCard::activate_relay(unsigned int relay) {
    if (relay < NUMBER_OF_RELAYS) {
      unsigned int bitToSet = (0x0001 << (NUMBER_OF_INPUTS + relay));
      outputData |= bitToSet;
      update_outputs();
    }
  }

  void IORelayCard::deactivate_relay(unsigned int relay) {
    if (relay < NUMBER_OF_RELAYS) {
      unsigned int bitToClear = ~(0x0001 << (NUMBER_OF_INPUTS + relay));
      outputData &= bitToClear;
      update_outputs();
    }
  }

  void IORelayCard::toggle_relay(unsigned int relay) {
    if (relay < NUMBER_OF_RELAYS) {
      unsigned int bitToToggle = (0x0001 << (NUMBER_OF_INPUTS + relay));
      outputData ^= bitToToggle;
      update_outputs();
    }
  }

  void IORelayCard::all_relays_off(void) {
    outputData = 0x0000;
    update_outputs();
  }

  void IORelayCard::update_outputs(void) {
    ioExpander.write(outputData);
  }

  void IORelayCard::update_inputs(void) {
    unsigned int data = ioExpander.read();
    for (unsigned int i = 0; i < NUMBER_OF_INPUTS; i++) {
      inputs[i]->set_state(determine_input_state(data, inputs[i]->get_id()));
    }
  }

  std::vector<Input*> IORelayCard::get_changed_inputs(void) {
    update_inputs();
    std::vector<Input*> changed;
    for (unsigned int i = 0; i < NUMBER_OF_INPUTS; i++) {
      if (inputs[i]->has_changed()) {
        changed.push_back(inputs[i]);
      }
    }
    return changed;
  }

};