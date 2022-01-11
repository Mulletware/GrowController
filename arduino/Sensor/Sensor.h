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
        this->movingAverage.begin();
        pinMode(inputChannel, INPUT); // temp sensor
      };
      sensor(int inputChannel, int movingAverageCount) {
        Sensor(inputChannel, PortType::analog, movingAverageCount);
      }
      Sensor() : movingAverage(0) {
        this->movingAverage.begin();
      }

      update() {
        switch(this->inputType) {
          case analog:
            this->currentValue = analogRead(this->inputChannel);
            break;
          case digital:
            this->currentValue = digitalRead(this->inputChannel);
            break;
        }

        this->movingAverage.reading(this->currentValue);
      };

      float getValue() {
        return this->currentValue;
      };

      float getMovingAverage() {
        return this->movingAverage.getAvg();
      }

      setInputChannel(int inputChannel) {
        this->inputChannel = inputChannel;
      }

    bool isValid = true;

    private:
      PortType inputType;
      int inputChannel;
      float currentValue;
    protected:
      movingAvgFloat movingAverage;
  };
}

#endif
