#ifndef SWITCH_H
#define SWITCH_H

#include "../Device/Device.h"
#include "../Types.h"

namespace GrowController {
  class Switch : public Device {
    public:
      Switch(int outputChannel) : Device(outputChannel, PortType::digital) { }

      void setPower(int power) {}
  };
}

#endif
