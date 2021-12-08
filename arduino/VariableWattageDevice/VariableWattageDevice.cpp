#include <RBDdimmer.h>
#include "../Device/Device.h"

namespace GrowController {

  class VariableWattageDevice : Device {
    public:
      VariableWattageDevice(int outputChannel) :
        Device(outputChannel, PortType::digital),
        dimmer(outputChannel)
      {
        // NO SERIAL.PRINT STATEMENTS CAN COME BEFORE THIS LINE RUNS OR THE PROGRAM WILL CRASH
        this->dimmer.begin(NORMAL_MODE, ON_OFF_typedef::OFF); // fuck this line in particular
      }

      setPower(int power) {
        if (power == 0) {
          this->turnOff();
        } else {
          this->turnOn();
        }

        if (this->getPower() != power) {
          this->dimmer.setPower(power);
        }
      }

      getPower() {
        return this->dimmer.getPower();
      }

      setMode(DIMMER_MODE_typedef mode) {
        this->dimmer.setMode(mode);
      }

      getMode() {
        this->dimmer.getMode();
      }

      setState(ON_OFF_typedef state) {
        this->dimmer.setState(state);
      }

      getState() {
        return this->dimmer.getState();
      }

      turnOn() {
        this->dimmer.setState(ON_OFF_typedef::ON);
      }

      turnOff() {
        this->dimmer.setState(ON_OFF_typedef::OFF);
      }

      bool isOn() {
        return Device::isOn();
      }

      dimmerLamp dimmer;
  };

}
