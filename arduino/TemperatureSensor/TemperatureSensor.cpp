#include <OneWire.h>
#include <DallasTemperature.h>
#include "../Sensor/Sensor.h";
#include "../PortType.h";
#include "./enums.h";

namespace GrowController {

  class TemperatureSensor : Sensor {
    public:
      TemperatureSensor(int inputChannel, temperatureUnit unit = fahrenheit)
        : Sensor(inputChannel, PortType::analog)
      {
        pinMode(inputChannel, INPUT);
        OneWire oneWire(inputChannel);

        this->oneWire = oneWire;
        DallasTemperature dTemp(&this->oneWire);

        this->dTemp = dTemp;
        this->dTemp.requestTemperatures();
        this->currentValue = this->dTemp.getTempFByIndex(0);

        this->setUnit(unit);
        this->update();
      }

      void update() {
        dTemp.requestTemperatures();
        currentValue = unit == celsius ? this->dTemp.getTempCByIndex(0) : this->dTemp.getTempFByIndex(0);
      }

      void setUnit(temperatureUnit newUnit) {
        unit = newUnit;
        this->update();
      }

      float getValue() {
        return this->currentValue;
      }

    private:
      OneWire oneWire;
      DallasTemperature dTemp;
      float currentValue;
      temperatureUnit unit;
  };

}
