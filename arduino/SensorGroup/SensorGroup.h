#ifndef SENSOR_GROUP_H
#define SENSOR_GROUP_H
#include <movingAvg.h>
#include <Array.h>
#include "../Sensor/Sensor.h"

namespace GrowController {

  template <typename SensorType, int MAX_SENSORS = 8>
  class SensorGroup {
    public:
      SensorGroup(
        int sensorPins[],
        int sensorCount,
        int movingAverageCount = 200
      ) : movingAverage(movingAverageCount)
      {
        this->movingAverage.begin();
        this->updateSensors(sensorPins, sensorCount);
      }

      void update() {
        for(int i = 0; i < this->sensorCount; i++) {
          this->sensors[i].update();
        }
      }

      float getValue() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          float value = this->sensors[i].getValue();

          if (this->sensors[i].isValid) {
            total += value;
          }
        }

        return total / (float)this->sensorCount;
      }

      float getMovingAverage() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          float value = this->sensors[i].getMovingAverage();

          Serial.print("isValid:"); Serial.print(this->sensors[i].isValid ? "true" : "false");
          Serial.print("this->sensors[");
          Serial.print(i);
          Serial.print("].getValue");
          Serial.println(this->sensors[i].getValue());

          if (this->sensors[i].isValid) {
            total += value;
          }
        }

        return total / (float)this->sensorCount;
      }

      void updateSensors(int sensorPins[], int sensorCount) {
        this->sensorCount = sensorCount;

        int total = min(MAX_SENSORS, sensorCount);
        for(int i = 0; i < total; i++) {
          SensorType sensor(sensorPins[i]);
          this->sensors[i] = sensor;
        }
      }

    private:
      SensorType sensors[MAX_SENSORS];
      int sensorCount;
      movingAvg movingAverage;
  };
}

#endif
