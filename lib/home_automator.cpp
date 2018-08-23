#include "home_automator.h"

#include "mbed.h"
#include "mqtt_message_factory.h"
#include "event_factory.h"
#include "simple_logger.h"

namespace BiosHomeAutomator {

  HomeAutomator::HomeAutomator(I2C * i2c, MQTTChannel * mqttChannel)
    : interrupt(IO_INTERRUPT_PIN), queue(QUEUE_SIZE * EVENTS_EVENT_SIZE) {
    info("Initializing Home Automator");

    this->i2c = i2c;
    setup_isr();
    this->mqttChannel = mqttChannel;
    this->mqttChannel->subscribe(MQTT_CARD_SUBSCRIBE_TOPIC, this, &HomeAutomator::handle_mqtt_message);

    warning("Setting default topic handler because MQTTThreadedClient does not handle wildcards well");
    this->mqttChannel->set_default_topic_handler(this, &HomeAutomator::handle_mqtt_message);
  }

  void HomeAutomator::setup_isr(void) {
    verbose("Setting up interrupt event queue");

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
        warning("ID" + std::to_string(relayCard->get_id()) + " for expansion card already taken");
        return;
      }
    }

    info("Registering IO RelayCard with id " + std::to_string(relayCard->get_id()));
    relayCards.push_back(relayCard);
  }

  void HomeAutomator::process_changed_inputs(void) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      std::vector<Input*> inputs = relayCards[i]->get_changed_inputs();
      for (unsigned int i = 0; i < inputs.size(); i++) {
        verbose(inputs[i]->to_string());

        MQTTMessage * message = MQTTMessageFactory::create_input_state_update(inputs[i]);
        if (message) {
          mqttChannel->publish(*message);
          delete message;
        } else {
          debug("Not publishing, pressed to long");
        }
      }
    }
  }

  void HomeAutomator::handle_mqtt_message(MQTT::MessageData& messageData) {
    std::string payload((char*)messageData.message.payload);

    std::string topic;
    if (messageData.topicName.lenstring.len > 0) {
      topic = std::string((const char *) messageData.topicName.lenstring.data, (size_t) messageData.topicName.lenstring.len);
    } else {
      topic = (const char *) messageData.topicName.cstring;
    }

    verbose("Handling received message: " + payload + "@ '" + topic + "'");

    MQTTMessage mqttMessage(topic, payload);
    CardEvent * event = EventFactory::create_event_from_mqtt_message(mqttMessage);
    if (event) {
      process_event(event);
    }
    delete event;
  }

  void HomeAutomator::process_event(CardEvent * event) {
    for (unsigned int i = 0; i < relayCards.size(); i++) {
      if (relayCards[i]->get_id() == event->get_expansion_card_id()) {
        event->apply_event_on(relayCards[i]);
        return;
      }
    }
  }

};