#ifndef TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#define TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.cpp"
#include "../SensorGroup/SensorGroup.h"

namespace GrowController {
  typedef SensorGroup<TemperatureHumiditySensor> TemperatureHumiditySensorGroup;

  TemperatureHumidtySensorGroup::getTemperature() {}
}

#endif
