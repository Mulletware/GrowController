#ifndef SENSOR_H
#define SENSOR_H
#include "../PortType.h"

using namespace PortType;

namespace Sensor {

  class Sensor
  {
    public:
      Sensor(int inputChannel, PortType::PortType inputType = PortType::PortType::digital)
      {
        this->inputChannel = inputChannel;
        this->inputType = inputType;
        pinMode(inputChannel, INPUT); // temp sensor
      };
      Sensor(int inputChannel);

      void update() {
        switch(this->inputType) {
          case analog:
            this->currentValue = analogRead(this->inputChannel);
            break;
          case digital:
            this->currentValue = digitalRead(this->inputChannel);
            break;
        }
      };

      int getValue() {
        return this->currentValue;
      };

    private:
      PortType::PortType inputType;
      int inputChannel;
      int currentValue;
  };


}

#endif
