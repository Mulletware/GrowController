#include "./enums.h"
#ifndef SENSOR_H
#define SENSOR_H

namespace Sensor {

  class Sensor
  {
    public:
      Sensor(int inputChannel, inputTypes inputType)
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
      inputTypes inputType;
      int inputChannel;
      int currentValue;
  };


}

#endif
