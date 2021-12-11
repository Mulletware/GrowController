#ifndef SOIL_MOISTURE_SENSOR
#define SOIL_MOISTURE_SENSOR

#include <OneWire.h>
#include <DallasTemperature.h>
#include <movingAvg.h>
#include "../Sensor/Sensor.h";
#include "../Types.h";
#include "./enums.h";

namespace GrowController {

  class SoilMoistureSensor : public Sensor {
    public:
      SoilMoistureSensor(
        int inputChannel = 0, // so that it can be consumed by Array (check this assumption)
        int wet = 300,
        int dry = 600,
        int movingAverageCount = 100
      ) : Sensor(inputChannel)
      {
        this->dry = dry;
        this->wet = wet;
        this->update();
      }

      update() {
        Sensor::update();
        Sensor::movingAverage.reading(Sensor::getValue());
      }

      int getValue() {
        return this->mapValue(Sensor::getValue());
      }

      int getMovingAverage() {
        return this->mapValue(Sensor::movingAverage.getAvg());
      }

      setInputChannel(int inputChannel) {
        Sensor::setInputChannel(inputChannel);
      }

    private:
      int dry, wet;

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
