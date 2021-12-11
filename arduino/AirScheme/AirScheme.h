#ifndef AIR_SCHEME_H
#define AIR_SCHEME_H
#include "../Relay/Relay.h"
#include "../VariableWattageFan/VariableWattageFan.h"
#include "../Types.h"

namespace GrowController {

  class AirScheme {
    public:
      AirScheme(Relay *heater, Relay *lights, Relay* humidifier, VariableWattageFan *fan) {
        this->heater = heater;
      }

      update(
        dayNightAirSettings_t tempSettings,
        dayNightAirSettings_t vpdSettings,
        wateringSettings_t wateringSettings
      ) {
        // if (moistureLevel < targetMoisture) {
        //   this->water->turnOn();
        // } else {
        //   this->water->turnOff();
        // }
      }

    private:
      Relay *heater;
      float targetTemp;
      float targetVPD;
  };

}

#endif
