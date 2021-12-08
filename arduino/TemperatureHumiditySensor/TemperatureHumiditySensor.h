#ifndef TEMPERATURE_HUMIDITY_SENSOR_H
#define TEMPERATURE_HUMIDITY_SENSOR_H

#include <Adafruit_BME280.h>
#include <Wire.h>
#include <movingAvg.h>

#include "../PortType.h"
#include "../I2CSensor/I2CSensor.h"
#include "../util.h";

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensor : I2CSensor {

    public:
      TemperatureHumiditySensor(
        int multiplexerAddress,
        int movingAverageCount = 30
      ) : I2CSensor(multiplexerAddress),
       temperatureMovingAvg(movingAverageCount),
       humidityMovingAvg(movingAverageCount)
       {
        I2CSensor::select();
        
        while (!this->bme.begin(0x76, &Wire)) {
          Serial.println("No BME280 found");
          delay(60000);
        }

        Serial.println("BME280 initialized!");

        temperatureMovingAvg.begin();
        humidityMovingAvg.begin();
      }

      update() {
        I2CSensor::select();

        this->temperature = bme.readTemperature();
        this->temperatureMovingAvg.reading(this->temperature * 100);

        this->humidity = bme.readHumidity();
        this->humidityMovingAvg.reading(this->humidity * 100);
      }

      float getTemperatureC() {
        return this->temperature;
      }

      float getTemperatureF() {
        return celsiusToF(this->getTemperatureC());
      }

      float getHumidity() {
        return this->humidity;
      }

      float getVPD() {
        return calculateVPD(this->temperature, this->humidity);
      }

      float getAverageVPD() {
        return calculateVPD(this->getAverageTemp(), this->getAverageHumidity());
      }

      float getAverageTemp() {
        return this->temperatureMovingAvg.getAvg() / 100.00;
      }

      float getAverageTempF() {
        return celsiusToF(this->getAverageTemp());
      }

      float getAverageHumidity() {
        return this->humidityMovingAvg.getAvg() / 100.00;
      }

    private:
      int inputChannel;
      Adafruit_BME280 bme;
      uint32_t delayMs;
      float temperature, humidity;
      movingAvg temperatureMovingAvg, humidityMovingAvg;
  };

}

#endif
