#ifndef TEMPERATURE_HUMIDITY_SENSOR_AHT10_H
#define TEMPERATURE_HUMIDITY_SENSOR_AHT10_H

#include <Wire.h>
#include <movingAvg.h>

#include <AHTxx.h>
#include "../PortType.h"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.h"
#include "../util.h";

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensorAHT10 : public TemperatureHumiditySensor {

    public:
      TemperatureHumiditySensorAHT10() {}
      TemperatureHumiditySensorAHT10(
        int multiplexerAddress,
        int movingAverageCount = 30
      )
      : TemperatureHumiditySensor(multiplexerAddress, movingAverageCount),
        aht(AHTXX_ADDRESS_X38, AHT1x_SENSOR)
       {
        while (!this->aht.begin()) {
          Serial.print("No AHT10 found at tca ");
          Serial.println(multiplexerAddress);
          delay(1000);
        }
      }

      update() {
        TemperatureHumiditySensor::update();

        TemperatureHumiditySensor::setTemperature(this->aht.readTemperature());
        TemperatureHumiditySensor::setHumidity(this->aht.readHumidity());
      }

    private:
      float temperature, humidity;
      AHTxx aht;
  };

}

#endif
