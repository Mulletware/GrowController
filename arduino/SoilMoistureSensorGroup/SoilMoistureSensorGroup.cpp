#ifndef SOIL_MOISTURE_SENSOR_GROUP_H
#define SOIL_MOISTURE_SENSOR_GROUP_H
#include "../SoilMoistureSensor/SoilMoistureSensor.cpp"
#include "../SensorGroup/SensorGroup.cpp"

namespace GrowController {
  typedef SensorGroup<SoilMoistureSensor> SoilMoistureSensorGroup;
}

#endif
