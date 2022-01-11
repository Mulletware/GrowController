#ifndef TEMPERATURE_HUMIDITY_SENSOR_H
#define TEMPERATURE_HUMIDITY_SENSOR_H

#include <AsyncDelay.h>
#include <movingAvg.h>
#include <Wire.h>
#include <avr/wdt.h>

#include "../Types.h"
#include "../I2CSensor/I2CSensor.h"
#include "../util.h";

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensor : I2CSensor {

    public:
      TemperatureHumiditySensor(
        int multiplexerAddress = 0,
        int readDelayDurationMs = 0,
        int movingAverageCount = 10
      ) : I2CSensor(multiplexerAddress),
       temperatureMovingAvg(movingAverageCount),
       humidityMovingAvg(movingAverageCount)
       {
        this->readDelayDurationMs = readDelayDurationMs;
        I2CSensor::select();

        temperatureMovingAvg.begin();
        humidityMovingAvg.begin();
      }

      void update() {
        if (this->shouldUpdate()) {
          I2CSensor::select();

          this->readDelay = AsyncDelay();
          this->readDelay.start(this->readDelayDurationMs, AsyncDelay::MILLIS);
        } else {
          Serial.println("Not updating!!");
        }
      }

      void select() {
        I2CSensor::select();
      }

      bool shouldUpdate() {
        return this->readDelay.isExpired();
      }

      float getTemperature() {
        return this->temperature;
      }

      float getHumidity() {
        return this->humidity;
      }

      float getVPD() {
        return calculateVPD(this->temperature, this->humidity);
      }

      float getAverageTemp() {
        return this->temperatureMovingAvg.getAvg() / 100.00;
      }

      float getAverageVPD() {
        return calculateVPD(this->getAverageTemp(), this->getAverageHumidity());
      }

      float getAverageHumidity() {
        return this->humidityMovingAvg.getAvg() / 100.00;
      }

      void setTemperature(float temp) {
        if (this->isValid) {
          this->temperature = temp;
          this->temperatureMovingAvg.reading(temp * 100);
        }
      }

      void setHumidity(float humidity) {
        if (this->isValid) {
          this->humidity = humidity;
          this->humidityMovingAvg.reading(humidity * 100);
        }
      }

      void setIsValid(bool valid) {
        if (valid && !this->isValid) { // going from invalid to valid
          this->temperatureMovingAvg.reset();
          this->humidityMovingAvg.reset();
        }

        this->isValid = valid;

        if (!valid) {
          this->isInitialized = false; // force reinitialize if invalid values are found
        }
      }

      bool
        isInitialized = true,
        isValid = true;

    private:
      int readDelayDurationMs;
      float temperature, humidity;
      movingAvg temperatureMovingAvg, humidityMovingAvg;
      AsyncDelay readDelay;
  };

}

#endif
