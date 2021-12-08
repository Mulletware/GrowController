#ifndef TEMPERATURE_HUMIDITY_SENSOR_SI7021_H
#define TEMPERATURE_HUMIDITY_SENSOR_SI7021_H

#include <Wire.h>
#include <movingAvg.h>

#include "Adafruit_Si7021.h"

#include "../PortType.h"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.h"
#include "../util.h";

#define DHTPIN A4;

namespace GrowController {

  class TemperatureHumiditySensorSI7021 : public TemperatureHumiditySensor {

    public:
      TemperatureHumiditySensorSI7021(
        int multiplexerAddress,
        int movingAverageCount = 30
      ) : TemperatureHumiditySensor(multiplexerAddress, movingAverageCount)
       {
        while (!this->sensor.begin()) {
          Serial.println("No SI7021 found");
          delay(1000);
        }
      }

      update() {
        TemperatureHumiditySensor::update();

        TemperatureHumiditySensor::setTemperature(this->sensor.readTemperature());

        TemperatureHumiditySensor::setHumidity(this->sensor.readHumidity());
      }

    private:
      float temperature, humidity;
      Adafruit_Si7021 sensor;
  };

}

#endif
