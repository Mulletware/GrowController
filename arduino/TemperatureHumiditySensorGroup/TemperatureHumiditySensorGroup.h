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

        for(int i = 0; i < min(MAX_SENSORS, sensorCount); i++) {
          SensorType sensor(tcaAddresses[i]);
          this->sensors[i] = sensor;
        }
      }

      update() {
        for(int i = 0; i < this->sensorCount; i++) {
          this->sensors[i].update();
        }
      }

      float getTemperatureC() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getTemperatureC();
        }

        return total / (float)this->sensorCount;
      }

      float getTemperatureF() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getTemperatureF();
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
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getAverageTemp();
        }

        return total / (float)this->sensorCount;
      }

      float getAverageTempF() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getAverageTempF();
        }

        return total / (float)this->sensorCount;
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

    private:
      SensorType sensors[MAX_SENSORS];
      int sensorCount;
      movingAvg movingAverage;
  };
}

#endif
