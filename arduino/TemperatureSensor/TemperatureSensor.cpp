#include <OneWire.h>
#include <DallasTemperature.h>
#include "../Sensor/Sensor.h";
#include "../Sensor/enums.h";
#include "./enums.h";

using namespace Sensor;

class TemperatureSensor : Sensor {
  public:
    TemperatureSensor(int inputChannel, temperatureUnit unit = fahrenheit)
      : Sensor(inputChannel, analog)
    {
      pinMode(inputChannel, INPUT);
      Serial.print("inputChannel: ");
      Serial.println(inputChannel);
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
      return currentValue;
    }

  private:
    OneWire oneWire;
    DallasTemperature dTemp;
    float currentValue;
    temperatureUnit unit;
};
