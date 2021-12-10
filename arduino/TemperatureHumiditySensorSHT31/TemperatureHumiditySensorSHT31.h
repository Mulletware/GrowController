#ifndef TEMPERATURE_HUMIDITY_SENSOR_SHT31_H
#define TEMPERATURE_HUMIDITY_SENSOR_SHT31_H

#include <Wire.h>
#include <movingAvg.h>

#include "SHT31.h"

#include "../PortType.h"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.h"
#include "../util.h";

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensorSHT31 : public TemperatureHumiditySensor {

    public:
      TemperatureHumiditySensorSHT31(
        int multiplexerAddress
      ) : TemperatureHumiditySensor(multiplexerAddress, 2000)
       {
        while (!this->sht.begin(0x44)) {
          Serial.println("No SHT31 found");
          delay(1000);
        }
      }

      update() {
        TemperatureHumiditySensor::update();

        this->sht.read(true);

        TemperatureHumiditySensor::setTemperature(this->sht.getTemperature());
        TemperatureHumiditySensor::setHumidity(this->sht.getHumidity());
      }

    private:
      float temperature, humidity;
      SHT31 sht;
  };

}

#endif
