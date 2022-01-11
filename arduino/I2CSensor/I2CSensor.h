#ifndef I2CSENSOR
#define I2CSENSOR
#include "Wire.h"
#include "../util.h"

namespace GrowController {
  class I2CSensor
  {
    public:
      I2CSensor() {}

      I2CSensor(int multiplexerAddress) {
        this->usesMultiplexer = true;
        this->multiplexerAddress = multiplexerAddress;
        this->select();
      }

      select() {
        if (this->usesMultiplexer == true) {
          tcaSelect(this->multiplexerAddress);
        }
      }

      int multiplexerAddress;

    private:
      bool usesMultiplexer = false;
  };
}

#endif
