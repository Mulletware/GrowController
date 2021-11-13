#ifndef SOIL_MOISTURE_SENSOR
#define SOIL_MOISTURE_SENSOR

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Array.h>
#include <movingAvgFloat.h>
#include "../Sensor/Sensor.h";
#include "../PortType.h";
#include "./enums.h";

namespace GrowController {

  class SoilMoistureSensor : public Sensor {
    public:
      SoilMoistureSensor(
        int inputChannel = -1, // so that it can be consumed by Array (check this assumption)
        int wet = 300,
        int dry = 600,
        int movingAverageCount = 30
      ) : Sensor(inputChannel),
          movingAverage(movingAverageCount)
      {
        this->dry = dry;
        this->wet = wet;
        this->update();
        this->movingAverage.begin();
      }

      update() {
        Sensor::update();
        this->movingAverage.reading(Sensor::getValue());
      }

      float getValue() {
        return this->mapValue(Sensor::getValue());
      }

      float getMovingAverage() {
        return this->mapValue(this->movingAverage.getAvg());
      }

      setInputChannel(int inputChannel) {
        Sensor::setInputChannel(inputChannel);
      }

    private:
      int dry, wet;

      movingAvgFloat movingAverage;

      int mapValue(float value) {
        bool isDryHigher = this->dry > this->wet;

        return max(0, min(100, map(
          value,
          this->dry,
          this->wet,
          isDryHigher ? 0 : 100,
          isDryHigher ? 100 : 0
        )));
      }
  };

}

#endif
