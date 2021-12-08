#ifndef TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#define TEMPERATURE_HUMIDITY_SENSOR_GROUP_H
#include "../TemperatureHumiditySensorBME280/TemperatureHumiditySensorBME280.cpp"
#include "../SensorGroup/SensorGroup.h"

namespace GrowController {
  typedef SensorGroup<TemperatureHumiditySensorBME280> TemperatureHumiditySensorBME280Group;

  TemperatureHumidtySensorGroup::getTemperature() {}
}

#endif
