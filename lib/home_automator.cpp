#include "home_automator.h"

#include "mbed.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  HomeAutomator::HomeAutomator(I2C * i2c)
    : interrupt(IO_INTERRUPT_PIN), queue(QUEUE_SIZE * EVENTS_EVENT_SIZE) {
#ifdef DO_SIMPLE_LOG
    Log.info("Initializing HomeAutomator");
#endif
    this->i2c = i2c;
    setup_isr();
  }

  void HomeAutomator::setup_isr(void) {
#ifdef DO_SIMPLE_LOG
    Log.verbose("Setting up interrupt event queue");
#endif
    // Start the event queue's dispatch thread
    eventDispatchThread.start(callback(&queue, &EventQueue::dispatch_forever));
    // Register handler via event queue (will run in user context)
    interrupt.fall(queue.event(callback(this, &HomeAutomator::process_changed_inputs)));
  }

  HomeAutomator::~HomeAutomator(void) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      delete relayCards[i];
    }
    relayCards.clear();
  }

  void HomeAutomator::add_card(IORelayCard * relayCard) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      if (relayCards[i]->get_id() == relayCard->get_id()) {
#ifdef DO_SIMPLE_LOG
        Log.warning("ID" + std::to_string(relayCard->get_id()) + " for expansion card already taken");
#endif
        return;
      }
    }

#ifdef DO_SIMPLE_LOG
    Log.info("Registering IO RelayCard with id " + std::to_string(relayCard->get_id()));
#endif
    relayCards.push_back(relayCard);
  }

  void HomeAutomator::process_changed_inputs(void) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      std::vector<Input*> inputs = relayCards[i]->get_changed_inputs();
      for (unsigned int i = 0; i < inputs.size(); i++) {
#ifdef DO_SIMPLE_LOG
        Log.verbose(inputs[i]->to_string());
#endif
      }
    }
  }


};