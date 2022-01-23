#ifndef WATERING_VALVE_DEVICE_H
#define WATERING_VALVE_DEVICE_H
#include <AsyncDelay.h>
#include "../Switch/Switch.h";

// watering is tricky.
// we want to deliver a short burst of water and let it settle in before
// allowing it to be turned on again
namespace GrowController {

  class WateringValveDevice : Switch {
    public:
      WateringValveDevice(
        int outputChannel,
        int wateringDurationMillis = 3000,
        int wateringRestrictedDurationMillis = 30000
      ) : Switch(outputChannel) {
        this->wateringDurationMillis = wateringDurationMillis;
        this->wateringRestrictedDurationMillis = wateringRestrictedDurationMillis;
      }

      turnOn() {
        if (this->wateringRestrictedDelay.isExpired()) {

          this->wateringRestrictedDelay = AsyncDelay();
          this->wateringRestrictedDelay.start(
            this->wateringRestrictedDurationMillis,
            AsyncDelay::MILLIS
          );
          this->waterOnDelay.start(
            this->wateringDurationMillis,
            AsyncDelay::MILLIS
          );

          if (!Switch::isOn()) {
            Switch::turnOn();
          }
        } else if (this->waterOnDelay.isExpired()) {
          if (Switch::isOn()) {
            this->turnOff();
          }
        }
      }

      turnOff() {
        Switch::turnOff();
      }

      setWateringDuration(int wateringDurationMillis = 3000) {
        this->wateringDurationMillis = wateringDurationMillis;
        this->waterOnDelay.expire();
      }

      setWateringRestrictedDuration(int wateringRestrictedDurationMillis = 30000) {
        this->wateringRestrictedDurationMillis = wateringRestrictedDurationMillis;
        this->wateringRestrictedDelay.expire();
      }

    private:
      AsyncDelay waterOnDelay, wateringRestrictedDelay;
      int wateringDurationMillis, wateringRestrictedDurationMillis;
  };

}

#endif
