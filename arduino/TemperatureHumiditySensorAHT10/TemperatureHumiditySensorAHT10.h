#ifndef TEMPERATURE_HUMIDITY_SENSOR_AHT10_H
#define TEMPERATURE_HUMIDITY_SENSOR_AHT10_H

#include <AHTxx.h>
#include "../Types.h"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.h"
#include "../util.h"
#include <avr/wdt.h>

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensorAHT10 : public TemperatureHumiditySensor {

    public:
      TemperatureHumiditySensorAHT10() {}
      TemperatureHumiditySensorAHT10(
        int multiplexerAddress
      )
      : TemperatureHumiditySensor(multiplexerAddress, 3000, 5),
        aht(AHTXX_ADDRESS_X38, AHT1x_SENSOR)
      {
        this->multiplexerAddress = multiplexerAddress;
        this->isInitialized = false;
        this->init();
      }

      bool init() {
        TemperatureHumiditySensor::select();

        if (this->aht.begin()) {
          this->isInitialized = true;
          return true;
        } else {
          Serial.print("No AHT10 found at tca ");
          Serial.println(this->multiplexerAddress);

          TemperatureHumiditySensor::setIsValid(false);
          return false;
        }
      }

       update() {
        if (!this->isInitialized) {
          this->init();
        }

        if (this->isInitialized && TemperatureHumiditySensor::shouldUpdate()) {
          TemperatureHumiditySensor::update();

          float temp = this->aht.readTemperature();

          TemperatureHumiditySensor::setIsValid(this->validate(temp));

          if (this->isValid) {
            TemperatureHumiditySensor::setTemperature(temp);
            TemperatureHumiditySensor::setHumidity(this->aht.readHumidity());
          } 
        }
      }

      bool validate(float value) {
        return value != 255.00 && !isNan(value) && value != INFINITY;
      }

    private:
      float temperature, humidity;
      int multiplexerAddress;
      AHTxx aht;
  };

}

#endif
