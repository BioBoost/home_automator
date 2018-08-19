#pragma once

#include "card_event.h"
#include "io_relay_card.h"

#ifdef DO_SIMPLE_LOG
#include "logger.h"
#endif

namespace BiosHomeAutomator {

  enum RelayEventType {
    ON,
    OFF,
    TOGGLE
  };

  class RelayEvent : public CardEvent {

    public:
      RelayEvent(unsigned int expansionCardId, unsigned int relayId, RelayEventType type)
        : CardEvent(expansionCardId) {

        this->relayId = relayId;
        this->type = type;
      }

      virtual ~RelayEvent(void) {}

      virtual void apply_event_on(ExpansionCard * card) override {
        if (card->get_id() == this->get_expansion_card_id()) {
          switch(type) {
            case ON:
              ((IORelayCard*)card)->activate_relay(relayId);
              break;
            case OFF:
              ((IORelayCard*)card)->deactivate_relay(relayId);
              break;
            case TOGGLE:
              ((IORelayCard*)card)->toggle_relay(relayId);
              break;
          }
        } else {
#ifdef DO_SIMPLE_LOG
          Log.warning("Trying to apply event on wrong expansion card");
#endif
        }
        
      }

    private:
      unsigned int relayId;
      RelayEventType type;

  };

};
