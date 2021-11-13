#ifndef SENSOR_GROUP_H
#define SENSOR_GROUP_H
#include <movingAvgFloat.h>
#include "../Sensor/Sensor.h"

namespace GrowController {

  template <class SensorType, int MAX_SENSORS = 16> class SensorGroup {
    public:
      SensorGroup(
        SensorType sensors[],
        int sensorCount,
        int movingAverageCount = 20
      ) : movingAverage(movingAverageCount) {
        this->sensorCount = sensorCount;

        for(int i = 0; i < sensorCount; i++) {
          this->sensors.push_back(sensors[i]);
          this->sensors[i].update();
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

        return total / this->sensorCount;
      }

      float getMovingAverage() {
        float total = 0;

        for(int i = 0; i < this->sensorCount; i++) {
          total += this->sensors[i].getMovingAverage();
        }

        return total / this->sensorCount;
      }

    private:
      Array<SoilMoistureSensor, MAX_SENSORS> sensors;
      int sensorCount;
      movingAvgFloat movingAverage;
  };
}

#endif
