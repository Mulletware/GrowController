#ifndef DEVICE_H
#define DEVICE_H

#include "../PortType.h"

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
      }
      Device(int outputChannel, char on, char off) {
        Device(outputChannel, PortType::analog, on, off);
      }

      set(int value) {
        Serial.print("value: ");
        Serial.println(value);
        switch (this->output) {
          case PortType::analog:
            analogWrite(this->outputChannel, value);
            break;
          case PortType::digital:
            digitalWrite(this->outputChannel, value);
            break;
        }
        this->currentValue = value;
      }

      turnOn() {
        set(this->ON);
      }

      turnOff() {
        set(this->OFF);
      }


    private:
      int outputChannel;
      PortType output;
      int currentValue;
      char OFF;
      char ON;
  };


}

#endif
