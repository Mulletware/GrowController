#ifndef GROW_CONTROLLER_T
#define GROW_CONTROLLER_T
#include <TimeLib.h>

namespace GrowController
{
  enum PortType
  {
    analog,
    digital
  };

  struct airSettings_t
  {
    float target;
    float min;
    float max;
  };

  struct dayNightAirSettings_t
  {
    airSettings_t day;
    airSettings_t night;
  };

  struct wateringSettings_t
  {
    int target;
    int onTime;
    int offTime;
  };

  struct lightingSettings_t
  {
    tmElements_t on;
    tmElements_t off;
  };

  struct controllerSettings_t
  {
    dayNightAirSettings_t temp;
    dayNightAirSettings_t vpd;
    wateringSettings_t watering;
    lightingSettings_t lighting;
  };

}

#endif
