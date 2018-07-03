#pragma once

namespace BiosHomeAutomator {

  class ExpansionCard {

    public:
      ExpansionCard(unsigned int id);
      virtual ~ExpansionCard(void);

    public:
      unsigned int get_id(void);

    private:
      unsigned int id;

  };

};