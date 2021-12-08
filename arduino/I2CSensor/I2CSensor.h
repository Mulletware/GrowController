#ifndef I2CSENSOR
#define I2CSENSOR
#include "Wire.h"
#include "../util.h"

namespace GrowController {
  class I2CSensor
  {
    public:
      I2CSensor() {}

      I2CSensor(int multiplexerPort) {
        this->usesMultiplexer = true;
        this->multiplexerPort = multiplexerPort;
        this->select();
      }

      select() {
        if (this->usesMultiplexer == true) {
          tcaSelect(this->multiplexerPort);
        }
      }

    private:
      bool usesMultiplexer = false;
      int multiplexerPort;
  };
}

#endif
