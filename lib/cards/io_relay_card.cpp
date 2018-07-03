#include "io_relay_card.h"

namespace BiosHomeAutomator {

  IORelayCard::IORelayCard(unsigned int ioExpanderI2cAddress, I2C * i2c, PinName ioInterrupt, unsigned int id)
    : ExpansionCard(id), ioExpander(ioExpanderI2cAddress, i2c, ioInterrupt) {

    ioExpander.set_port_direction(EXPANDER_PORT_MASK);
    all_relays_off();
  }

  IORelayCard::~IORelayCard(void) { }

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

};