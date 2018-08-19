#pragma once

#include "expansion_card.h"

namespace BiosHomeAutomator {

  class CardEvent {

    public:
      CardEvent(unsigned int expansionCardId) {
        this->expansionCardId = expansionCardId;
      }

      virtual ~CardEvent(void) {}

    public:
      unsigned int get_expansion_card_id(void) {
        return expansionCardId;
      }

    public:
      virtual void apply_event_on(ExpansionCard * card) = 0;

    private:
      unsigned int expansionCardId;
  };

};
