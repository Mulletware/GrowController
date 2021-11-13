#ifndef WATERING_VALVE_DEVICE
#define WATERING_VALVE_DEVICE
#include <AsyncDelay.h>
#include "../Relay/Relay.cpp";

// watering is tricky.
// we want to deliver a short burst of water and let it settle in before
// allowing it to be turned on again
namespace GrowController {

  class WateringValveDevice : Relay {
    public:
      WateringValveDevice(int outputChannel) : Relay(outputChannel) { }

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
          Relay::turnOn();
        } else if (this->waterOnDelay.isExpired()) {
          this->turnOff();
        }
      }

      turnOff() {
        Relay::turnOff();
      }

    private:
      AsyncDelay waterOnDelay, wateringRestrictedDelay;
      int
        wateringDurationMillis = 2000,
        wateringRestrictedDurationMillis = 15000;
  };

}

#endif
