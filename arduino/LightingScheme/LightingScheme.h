#ifndef LIGHTING_SCHEME
#define LIGHTING_SCHEME
#include "../Relay/Relay.h"
#include "../util.h"

namespace GrowController {

  class LightingScheme {
    public:
      LightingScheme(Relay *lights) { // todo: add light intensity here
        this->lights = lights;
      }

      update(tmElements_t time, tmElements_t onTime, tmElements_t offTime) {
        // 0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15 - 16 - 17 - 18 - 19 - 20 - 21 - 22 - 23
        //                                  off                                                      on
        //                                                                                               | <-now

        // 0 - 1 - 2 - 3 - 4 - 5 - 6 - 7 - 8 - 9 - 10 - 11 - 12 - 13 - 14 - 15 - 16 - 17 - 18 - 19 - 20 - 21 - 22 - 23
        //     on                                                 off
        //                                         | <-now

        // ^ use these examples to step "now" through each hour to better understand this logic

        if (isAfter(onTime, offTime)) { // turns off first
          if (isBetween(time, offTime, onTime)) {
            this->lights->turnOff();
          } else {
            this->lights->turnOn();
          }
        } else {
          if (isBetween(time, onTime, offTime)) {
            this->lights->turnOn();
          } else {
            this->lights->turnOff();
          }
        }
      }

    private:
      Relay *lights;
  };

}

#endif
