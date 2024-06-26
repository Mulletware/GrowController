#ifndef DEVICE_H
#define DEVICE_H

#include "../Types.h"

namespace GrowController {

  class Device {
    public:
      Device(
        int outputChannel,
        PortType output = PortType::analog,
        char on = HIGH,
        char off = LOW
      ) {
        pinMode(outputChannel, OUTPUT);
        this->outputChannel = outputChannel;
        this->output = output;
        this->ON = on;
        this->OFF = off;
        this->turnOff();
      }
      Device(int outputChannel, char on, char off) {
        Device(outputChannel, PortType::analog, on, off);
      }

      void set(char value) {
        switch (this->output) {
          case PortType::analog:
            analogWrite(this->outputChannel, value);
            break;
          case PortType::digital:
            digitalWrite(this->outputChannel, value);
            break;
        }
        this->currentValue = value;
        this->isSet = true;
      }

      void turnOn() {
        if (!this->on || !this->isSet) {
          this->set(this->ON);
          this->on = true;
        }
      }

      void turnOff() {
        if (this->on || !this->isSet) {
          this->set(this->OFF);
          this->on = false;
        }
      }

      bool isOn() {
        return this->on;
      }

    private:
      int outputChannel;
      PortType output;
      int currentValue;
      char OFF;
      char ON;
      bool on = false;
      bool isSet = false;
  };

}

#endif
