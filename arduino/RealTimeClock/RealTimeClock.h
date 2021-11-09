#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#include "../Sensor/Sensor.h"

namespace GrowController {

  class RealTimeClock : Sensor {
    public:
      RealTimeClock();
      getTime();
      update();
      getValue();
  };

}

#endif
