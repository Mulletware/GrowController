#ifndef WATERING_SCHEME_H
#define WATERING_SCHEME_H
#include "../WateringValveDevice/WateringValveDevice.h"

namespace GrowController {

  class WateringScheme {
    public:
      WateringScheme(WateringValveDevice *water) {
        this->water = water;
      }

      update(int moistureLevel, int targetMoisture) {
        if (moistureLevel < targetMoisture) {
          Serial.println("Turning on watering!!!");
          this->water->turnOn();
        } else {
          this->water->turnOff();
        }
      }

    private:
      WateringValveDevice *water;
  };

}

#endif
