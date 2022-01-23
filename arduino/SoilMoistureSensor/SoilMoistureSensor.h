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
        int wet = 200,
        int dry = 700,
        int movingAverageCount = 100
      ) : Sensor(inputChannel)
      {
        this->dry = dry;
        this->wet = wet;
        this->update();
      }

      void update() {
        Sensor::update();
        int value = Sensor::getValue();
        if (this->validate(value)) {
          Sensor::movingAverage.reading(value);
        } else {
          Sensor::movingAverage.reset();
        }
      }

      int getValue() {
        int value = Sensor::getValue();
        Serial.print("value: "); Serial.println(value);
        this->validate(value);
        return this->mapValue(value);
      }

      int getMovingAverage() {
        return this->mapValue(Sensor::movingAverage.getAvg());
      }

      void setInputChannel(int inputChannel) {
        Sensor::setInputChannel(inputChannel);
      }

      bool validate(int value) {
        Serial.print("value: "); Serial.println(value);
        int variance = abs(this->wet - this->dry) / 2;
        this->isValid = value < 750;
        // value < max(this->wet, this->dry) + variance
        //   && value > min(this->wet, this->dry) - variance;
        return this->isValid;
      }
      
      bool isValid = true;

    private:
      int dry, wet;

      int mapValue(float value) {
        bool isDryHigher = this->dry > this->wet;

        return constrain(map(
          value,
          this->dry,
          this->wet,
          isDryHigher ? 0 : 100,
          isDryHigher ? 100 : 0
        ), 0, 100);
      }
  };

}

#endif
