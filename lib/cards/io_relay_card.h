#pragma once

#include "expansion_card.h"
#include "pcf8575.h"
#include "input.h"
#include <vector>

namespace BiosHomeAutomator {

  class IORelayCard : public ExpansionCard {

    public:
      IORelayCard(unsigned int ioExpanderI2cAddress, I2C * i2c, PinName ioInterrupt, unsigned int id);
      virtual ~IORelayCard(void);

    public:
      void activate_relay(unsigned int relay);
      void deactivate_relay(unsigned int relay);
      void toggle_relay(unsigned int relay);
      void all_relays_off(void);
      std::vector<Input*> get_changed_inputs(void);

    private:
      void initialize_inputs(void);
      void update_inputs(void);
      Input::State determine_input_state(unsigned int portState, unsigned int inputId);

    private:
      void update_outputs(void);

    private:
      static const unsigned int NUMBER_OF_INPUTS = 10;
      static const unsigned int NUMBER_OF_RELAYS = 6;
      static const unsigned int EXPANDER_PORT_MASK = 0x03FF;

    private:
      IOExpansion::PCF8575 ioExpander;
      unsigned int outputData;
      std::vector<Input*> inputs;
  };

};