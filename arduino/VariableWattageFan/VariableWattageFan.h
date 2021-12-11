#ifndef VARIABLE_WATTAGE_FAN_H
#define VARIABLE_WATTAGE_FAN_H
#include <AsyncDelay.h>
#include "../VariableWattageDevice/VariableWattageDevice.h";

namespace GrowController {

  class VariableWattageFan : VariableWattageDevice {
    public:
      VariableWattageFan(int outputChannel, int low = 40, int high = 90)
        : VariableWattageDevice(outputChannel)
      {
        this->low = low;
        this->high = high;
        this->isStartingFan = false;
      }

      setPower(int power) {
        if (this->isStartingFan == true) {
          if (this->fanStartDelay.isExpired()) {
            this->isStartingFan = false;
            this->fanStartDelay = AsyncDelay();
            if (this->getPower() != power) {
              this->setPower(power);
            }
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
          if (this->fanPower > 0) {
            this->fanPower = 0;
            VariableWattageDevice::setPower(0);

            this->turnOffDelay = AsyncDelay();
            this->turnOffDelay.start(30000, AsyncDelay::MILLIS);
          }
          if (this->isOn()) {
            this->turnOff();
          }
        } else {
          this->fanPower = power;
          VariableWattageDevice::setPower(this->mapPower(power));
        }
      }

      int mapPower(int power) {
        return map(power, 0, 100, this->low + 1, this->high);
      }

      startFromStopped(int power) {
        // do startup routine to ensure motor spins
        // if power is less than 60, rev motor to 75 for 2 seconds (async delay) then set to true power

        if (this->turnOffDelay.isExpired()) {
          this->turnOn();

          this->fanPower = power;
          if (power < 50) {
            this->isStartingFan = true;
            this->fanStartDelay.start(300, AsyncDelay::MILLIS);
            VariableWattageDevice::setPower(this->mapPower(50));
          } else {
            VariableWattageDevice::setPower(this->mapPower(power));
          }
        }
      }

      getPower() {
        return this->mapPower(VariableWattageDevice::getPower());
      }

      turnOn() {
        VariableWattageDevice::turnOn();
      }

      turnOff() {
        VariableWattageDevice::turnOff();
      }

      bool isOn() {
        return VariableWattageDevice::isOn();
      }


    private:
      int low, high, currentPower;
      int fanPower = 0;
      AsyncDelay fanStartDelay;
      AsyncDelay turnOffDelay;
      bool isStartingFan;
  };

}

#endif
