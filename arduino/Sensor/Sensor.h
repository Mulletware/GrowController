#ifndef SENSOR_H
#define SENSOR_H
#include <movingAvgFloat.h>
#include "../PortType.h"

namespace GrowController {

  class Sensor
  {
    public:
      Sensor(
        int inputChannel,
        PortType inputType = PortType::analog,
        int movingAverageCount = 20
      ) : movingAverage(movingAverageCount) {
        this->inputChannel = inputChannel;
        this->inputType = inputType;
        pinMode(inputChannel, INPUT); // temp sensor
      };
      Sensor() : movingAverage(0) {}

      virtual update() {
        switch(this->inputType) {
          case analog:
            this->currentValue = analogRead(this->inputChannel);
            break;
          case digital:
            this->currentValue = digitalRead(this->inputChannel);
            break;
        }
      };

      virtual float getValue() {
        // Serial.print("this->currentValue: ");
        // Serial.println(this->currentValue);

        return this->currentValue;
      };

      virtual float getMovingAverage() {
        return this->movingAverage.getAvg();
      }

      setInputChannel(int inputChannel) {
        this->inputChannel = inputChannel;
      }

    private:
      PortType inputType;
      int inputChannel;
      float currentValue;
      movingAvgFloat movingAverage;
  };
}

#endif
