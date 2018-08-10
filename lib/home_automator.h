#pragma once

#include <vector>
#include "io_relay_card.h"
#include "mqtt_channel.h"

namespace BiosHomeAutomator {

  class HomeAutomator {

    public:
      HomeAutomator(I2C * i2c, MQTTChannel * mqttChannel);
      virtual ~HomeAutomator(void);
    
    public:
      void add_card(IORelayCard * card);

    private:
      void setup_isr(void);
      void process_changed_inputs(void);

    private:
      I2C * i2c;
      std::vector<IORelayCard *> relayCards;
      MQTTChannel * mqttChannel;

      // Interrupt stuff
      static const unsigned int QUEUE_SIZE = 12;
      InterruptIn interrupt;
      EventQueue queue;
      Thread eventDispatchThread;
  };

};