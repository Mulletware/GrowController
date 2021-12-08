#ifndef SOIL_MOISTURE_SENSOR_GROUP_H
#define SOIL_MOISTURE_SENSOR_GROUP_H
#include "../SoilMoistureSensor/SoilMoistureSensor.h"
#include <movingAvg.h>
#include "../SensorGroup/SensorGroup.h"
const int MAX_SENSORS = 8;

namespace GrowController {
  typedef SensorGroup<SoilMoistureSensor> SoilMoistureSensorGroup;
}

#endif
