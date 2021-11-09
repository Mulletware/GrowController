#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <math.h>
#include "../PortType.h"

#define DHTPIN A4;

namespace GrowController {

  // TemperatureHumiditySensor::TemperatureHumiditySensor() : dht(inputChannel, DHT22) { }


  class TemperatureHumiditySensor : Sensor {
    public:
      TemperatureHumiditySensor(int inputChannel)
        : Sensor(inputChannel, PortType::analog),
          dht(inputChannel, DHT22)
      {
        this->inputChannel = inputChannel;
        dht.begin();

        sensor_t sensor;
        this->delayMs = sensor.min_delay / 1000;
        this->dht.temperature().getSensor(&sensor);
        this->dht.humidity().getSensor(&sensor);
      }

      update() {
        sensors_event_t event;
        dht.temperature().getEvent(&event);

        if (isnan(event.temperature)) {
          Serial.println(F("Error reading temperature!"));
        } else {
          Serial.print(F("Temperature: "));
          Serial.print(event.temperature);
          Serial.println(F("°C"));

          this->temperature = event.temperature;
        }
        // Get humidity event and print its value.
        dht.humidity().getEvent(&event);
        if (isnan(event.relative_humidity)) {
          Serial.println(F("Error reading humidity!"));
        } else {
          Serial.print(F("Humidity: "));
          Serial.print(event.relative_humidity);
          Serial.println(F("%"));

          this->humidity = event.relative_humidity;
        }
        // Serial.println(this->dht->humidity());
      }

      float getTemperatureC() {
        return this->temperature;
      }

      float getTemperatureF() {
        return getTemperatureC() * 1.8 + 32;
      }

      float getHumidity() {
        return this->humidity;
      }

      float getVPD() {
        return
          (610.7 * pow(10,
            ((7.5 * this->temperature) /
            (237.3 + this->temperature)))
          )
          / 1000
          * (this->humidity / 100);

      }

    private:
      int inputChannel;
      DHT_Unified dht;
      float temperature;
      float humidity;
      uint32_t delayMs;
  };

}
