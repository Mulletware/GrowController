#ifndef TEMPERATURE_HUMIDITY_SENSOR_H
#define TEMPERATURE_HUMIDITY_SENSOR_H

#include <AsyncDelay.h>
#include <movingAvg.h>
#include <Wire.h>

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

      update() {
        if (this->shouldUpdate()) {
          I2CSensor::select();

          this->readDelay = AsyncDelay();
          this->readDelay.start(this->readDelayDurationMs, AsyncDelay::MILLIS);
        }
      }

      bool shouldUpdate() {
        return this->readDelay.isExpired();
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

      float getAverageTemp() {
        return this->temperatureMovingAvg.getAvg() / 100.00;
      }

      float getAverageTempF() {
        return celsiusToF(this->getAverageTemp());
      }

      float getAverageVPD() {
        return calculateVPD(this->getAverageTemp(), this->getAverageHumidity());
      }

      float getAverageHumidity() {
        return this->humidityMovingAvg.getAvg() / 100.00;
      }

      setTemperature(float temp) {
        this->temperature = temp;
        this->temperatureMovingAvg.reading(this->temperature * 100);
      }

      setHumidity(float humidity) {
        this->humidity = humidity;
        this->humidityMovingAvg.reading(this->humidity * 100);
      }

    private:
      int inputChannel;
      int readDelayDurationMs;
      float temperature, humidity;
      movingAvg temperatureMovingAvg, humidityMovingAvg;
      AsyncDelay readDelay;
  };

}

#endif
