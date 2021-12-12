#ifndef LIGHTING_SCHEME_H
#define LIGHTING_SCHEME_H
#include "../Relay/Relay.h"
#include "../util.h"
#include "../Types.h"

namespace GrowController {

  class LightingScheme {
    public:
      LightingScheme(Relay *lights) { // todo: add light intensity here
        this->lights = lights;
      }

      update(tmElements_t time, lightingSettings_t settings) {

        if (isDay(time, settings.on, settings.off)) {
            this->lights->turnOn();
        } else {
            this->lights->turnOff();
        }

      }

    private:
      Relay *lights;
  };

}

#endif
