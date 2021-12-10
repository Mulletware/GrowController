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
        this->sensorCount = sensorCount;
        this->movingAverage.begin();

        for(int i = 0; i < min(MAX_SENSORS, sensorCount); i++) {
          SensorType sensor(sensorPins[i]);
          this->sensors[i] = sensor;
        }
      }

      update() {
        for(int i = 0; i < this->sensorCount; i++) {
          this->sensors[i].update();
        }
      }

      float getValue() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getValue();
        }

        return total / (float)this->sensorCount;
      }

      float getMovingAverage() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          float value = this->sensors[i].getMovingAverage();
          total += value;
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
