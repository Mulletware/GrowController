#ifndef WATERING_SCHEME
#define WATERING_SCHEME
#include "../WateringValveDevice/WateringValveDevice.cpp"

namespace GrowController {

  class WateringScheme {
    public:
      WateringScheme(WateringValveDevice *water) {
        this->water = water;
      }

      update(int moistureLevel, int targetMoisture) {
        if (moistureLevel < targetMoisture) {
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
