#ifndef RELAY_H
#define RELAY_H
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

    private:
      bool isOn = false;
  };

}

#endif
