#ifndef RELAY
#define RELAY
#include "../Device/Device.h"

namespace GrowController {

  class Relay : public Device {
    public:
      Relay(int outputChannel) :
        Device(outputChannel, PortType::digital, LOW, HIGH) {}

      turnOn() {
        Device::turnOn();
      }

      turnOff() {
        Device::turnOff();
      }

  };

}

#endif
