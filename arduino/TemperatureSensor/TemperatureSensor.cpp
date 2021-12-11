#include <OneWire.h>
#include <DallasTemperature.h>
#include <movingAvgFloat.h>
#include "../Sensor/Sensor.h";
#include "../Types.h";
#include "./enums.h";

namespace GrowController {

  class TemperatureSensor : Sensor {
    public:
      TemperatureSensor(int inputChannel, temperatureUnit unit = fahrenheit)
        : Sensor(inputChannel)
      {
        pinMode(inputChannel, INPUT);
        OneWire oneWire(inputChannel);

        this->oneWire = oneWire;
        DallasTemperature dTemp(&this->oneWire);

        this->dTemp = dTemp;

        this->setUnit(unit);
        this->update();
      }

      update() {
        this->dTemp.requestTemperatures();

        currentValue = unit == celsius
          ? this->dTemp.getTempCByIndex(0)
          : this->dTemp.getTempFByIndex(0);
      }

      setUnit(temperatureUnit newUnit) {
        this->unit = newUnit;
        this->update();
      }

      float getValue() {
        return Sensor::getValue();
      }

    private:
      OneWire oneWire;
      DallasTemperature dTemp;
      float currentValue;
      temperatureUnit unit;

  };

}
