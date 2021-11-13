#include <AsyncDelay.h>
#include "../VariableWattageDevice/VariableWattageDevice.cpp";

namespace GrowController {

  class VariableWattageFan : VariableWattageDevice {
    public:
      VariableWattageFan(int outputChannel, int low = 40, int high = 90)
        : VariableWattageDevice(outputChannel)
      {
        this->low = low;
        this->high = high;
        this->isStartingFan = false;
        this->turnOff();
      }

      setPower(int power) {

        if (this->isStartingFan == true) {
          if (this->fanStartDelay.isExpired()) {
            this->isStartingFan = false;
            this->fanStartDelay = AsyncDelay();
            this->setPower(power);
          }
        } else if (
          VariableWattageDevice::getPower() == 0
            && VariableWattageDevice::getMode() == ON_OFF_typedef::OFF
            && power > 0
        ) {
          // if we're switching from off to on
          //    run the motor full speed for a split second
          this->startFromStopped(power);
        } else if (power <= 0) {

          VariableWattageDevice::setPower(0);
          this->turnOff();
        } else {
          VariableWattageDevice::setPower(this->mapPower(power));
        }
      }

      int mapPower(int power) {
        return map(power, 0, 100, this->low + 1, this->high);
      }

      startFromStopped(int power) {
        // do startup routine to ensure motor spins
        // if power is less than 60, rev motor to 75 for 2 seconds (async delay) then set to true power

        this->turnOn();

        if (power < 50) {
          this->isStartingFan = true;
          this->fanStartDelay.start(300, AsyncDelay::MILLIS);
          VariableWattageDevice::setPower(this->mapPower(50));
        } else {
          VariableWattageDevice::setPower(this->mapPower(power));
        }
      }

      getPower() {
        return this->mapPower(VariableWattageDevice::getPower());
      }

      turnOn() {
        VariableWattageDevice::turnOn();
      }

      turnOff() {
        VariableWattageDevice::setPower(0);
        VariableWattageDevice::turnOff();
      }


    private:
      int low, high, currentPower;
      AsyncDelay fanStartDelay;
      bool isStartingFan;
  };

}
