#ifndef RELAY
#define RELAY
#include "../Device/Device.h"

namespace GrowController {

  class Relay : public Device {
    public:
      Relay(int outputChannel) :
        Device(outputChannel, PortType::digital, LOW, HIGH) {}

      turnOn() {
        if (!this->isOn) {
          Device::turnOn();
          this->isOn = true;
        }
      }

      turnOff() {
        if (this->isOn) {
          Device::turnOff();
          this->isOn = false;
        }
      }

      bool isOn = false;
  };

}

#endif
