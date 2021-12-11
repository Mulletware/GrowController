#ifndef WATERING_VALVE_DEVICE_H
#define WATERING_VALVE_DEVICE_H
#include <AsyncDelay.h>
#include "../Relay/Relay.h";

// watering is tricky.
// we want to deliver a short burst of water and let it settle in before
// allowing it to be turned on again
namespace GrowController {

  class WateringValveDevice : Relay {
    public:
      WateringValveDevice(
        int outputChannel,
        int wateringDurationMillis = 3000,
        int wateringRestrictedDurationMillis = 30000
      ) : Relay(outputChannel) {
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

          if (!Relay::isOn) {
            Relay::turnOn();
          }
        } else if (this->waterOnDelay.isExpired()) {
          if (Relay::isOn) {
            this->turnOff();
          }
        }
      }

      turnOff() {
        Relay::turnOff();
      }

    private:
      AsyncDelay waterOnDelay, wateringRestrictedDelay;
      int wateringDurationMillis, wateringRestrictedDurationMillis;
  };

}

#endif
