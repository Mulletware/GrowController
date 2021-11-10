#include "../VariableWattageDevice/VariableWattageDevice.cpp";

namespace GrowController {

  class VariableWattageFan : VariableWattageDevice {
    public:
      VariableWattageFan(int outputChannel, int low = 40, int high = 90)
        : VariableWattageDevice(outputChannel)
      {
        this->low = low;
        this->high = high;
      }

      setPower(int power) {
        if (this->getPower() == 0) {
          Serial.println(this->getPower());
          this->startFromStopped(this->mapPower(power));
        } else {
          VariableWattageDevice::setPower(this->mapPower(power));
        }
      }

      int mapPower(int power) {
        return map(power, 0, 100, this->low, this->high);
      }

      startFromStopped(int power) {
        // do startup routine to ensure motor spins
        // if power is less than 60, rev motor to 75 for 2 seconds (async delay) then set to true power
        VariableWattageDevice::setPower(power);
      }

      getPower() {
        VariableWattageDevice::getPower();
      }


    private:
      int low, high, currentPower;
  };

}
