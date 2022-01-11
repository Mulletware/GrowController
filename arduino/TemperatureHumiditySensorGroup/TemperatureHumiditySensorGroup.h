#ifndef TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#define TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#include <movingAvg.h>
#include <Array.h>
#include "../Sensor/Sensor.h"

namespace GrowController {

  template <typename SensorType, int MAX_SENSORS = 8>
  class TemperatureHumiditySensorGroup {
    public:
      TemperatureHumiditySensorGroup(
        int tcaAddresses[],
        int sensorCount,
        int movingAverageCount = 200
      ) : movingAverage(movingAverageCount)
      {
        this->sensorCount = sensorCount;
        this->movingAverage.begin();

        int total = min(MAX_SENSORS, sensorCount);
        for(int i = 0; i < total; i++) {
          SensorType sensor(tcaAddresses[i]);
          this->sensors[i] = sensor;
        }
      }

      update() {
        for(int i = 0; i < this->sensorCount; i++) {
          this->sensors[i].update();
        }
      }

      float getTemperature() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getTemperature();

        }

        return total / (float)this->sensorCount;
      }

      float getHumidity() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getHumidity();
        }

        return total / (float)this->sensorCount;
      }

      float getVPD() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getVPD();
        }

        return total / (float)this->sensorCount;
      }

      float getAverageTemp() {
        Serial.println("***************     Getting Average Temp    *********************** ");
        float total = 0;
        int count = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          float temp = this->sensors[i].getAverageTemp();

          Serial.print("this->sensors[");
          Serial.print(i);
          Serial.print("].isValid: ");
          Serial.println(this->sensors[i].isValid ? "true" : "false");

          if (this->sensors[i].isValid) {
            total += temp;
            count++;
          }

          Serial.print("temp sensor ");
          Serial.print(i);
          Serial.print(": ");
          Serial.println(this->sensors[i].getTemperature());
        }

        this->hasErrors = count != this->sensorCount;

        return total / (float)count;
      }

      float getAverageHumidity() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getAverageHumidity();
        }

        return total / (float)this->sensorCount;
      }

      float getAverageVPD() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getAverageVPD();
        }

        return total / (float)this->sensorCount;
      }

      bool hasErrors;

    private:
      SensorType sensors[MAX_SENSORS];
      int sensorCount;
      movingAvg movingAverage;
  };
}

#endif
