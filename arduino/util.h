#ifndef UTIL_H
#define UTIL_H
#include <math.h>

namespace GrowController {
  float calculateVPD(float temperature, float humidity) {
    return (610.7 * pow(10,
        ((7.5 * temperature) /
        (237.3 + temperature)))
      )
      / 1000 * (humidity / 100);
  }

}

#endif
