#include "../Sensor/Sensor.h";

namespace GrowController {

  class DummySensor {
    public:
      DummySensor(float low = 69.90, float high = 70.4, int persistence = 1)
      {
        this->low = low;
        this->high = high;
        this->currentValue = low;
        this->isIncreasing = true;
        this->persistence = persistence;
        this->persist = persistence;
      };

      update() {

        if (persist == persistence) {
          if (this->isIncreasing) {
            this->currentValue += 0.01;
            if (this->currentValue >= this->high) {
              this->isIncreasing = false;
            }
          } else {
            this->currentValue -= 0.01;
            if (this->currentValue <= this->low) {
              this->isIncreasing = true;
            }
          }

          this->persist = 0;
        } else {
          this->persist++;
        }
      }

      float getValue() {
        return this->currentValue / 1.00;
      }

    private:
      bool isIncreasing;
      int persist, persistence;
      float low, high, currentValue;
  };

}
