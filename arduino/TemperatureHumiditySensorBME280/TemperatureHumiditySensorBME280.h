#ifndef TEMPERATURE_HUMIDITY_SENSOR_BME280_H
#define TEMPERATURE_HUMIDITY_SENSOR_BME280_H

#include <Adafruit_BME280.h>
#include <Wire.h>
#include <movingAvg.h>

#include "../PortType.h"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.h"
#include "../I2CSensor/I2CSensor.h"
#include "../util.h";

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensorBME280 : public TemperatureHumiditySensor {

    public:
      TemperatureHumiditySensorBME280(
        int multiplexerAddress
      ) : TemperatureHumiditySensor(multiplexerAddress)
       {
        while (!this->bme.begin(0x76, &Wire)) {
          Serial.println("No BME280 found");
          delay(1000);
        }

        Serial.println("BME280 initialized!");
      }

      update() {
        TemperatureHumiditySensor::update();

        TemperatureHumiditySensor::setTemperature(this->bme.readTemperature());
        
        TemperatureHumiditySensor::setHumidity(
          this->humidity = this->bme.readHumidity()
        );
      }

    private:
      Adafruit_BME280 bme;
      float temperature, humidity;
  };

}

#endif
