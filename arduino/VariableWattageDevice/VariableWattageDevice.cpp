#include <RBDdimmer.h>
#include "../Device/Device.h"

namespace GrowController {

  class VariableWattageDevice : Device {
    public:
      VariableWattageDevice(int outputChannel) :
        Device(outputChannel, PortType::digital),
        dimmer(outputChannel)
      {
        this->dimmer.begin(NORMAL_MODE, ON_OFF_typedef::OFF);
      }

      setPower(int power) {
        // Serial.print("running parent ");
        // Serial.println(power);

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

      turnUp() {}

      turnDown() {}

      dimmerLamp dimmer;
  };

}
