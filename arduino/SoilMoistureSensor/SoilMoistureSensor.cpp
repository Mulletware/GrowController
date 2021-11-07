#include <OneWire.h>
#include <DallasTemperature.h>
#include "../Sensor/Sensor.h";
#include "../Sensor/enums.h";
#include "./enums.h";

using namespace Sensor;

class SoilMoistureSensor : Sensor {
  public:
    SoilMoistureSensor(int inputChannel, int dry = 600, int wet = 300)
      : Sensor(inputChannel, analog)
    {
      this->dry = dry;
      this->wet = wet;
      this->update();
    }

    update() {
      Sensor::update();
    }

    getValue() {
      return this->mapValue(Sensor::getValue());
    }

  private:
    int dry, wet;

    int mapValue(int value) {
      bool isDryHigher = this->dry > this->wet;

      return isDryHigher
        ? map( value, this->wet, this->dry, 100, 0)
        : map( value, this->dry, this->wet, 0, 100);

    }
};
