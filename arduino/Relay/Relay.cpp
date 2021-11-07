#include "../Device/Device.h";

class Relay : Device {
  public:
    Relay(int inputChannel) :
      Device(inputChannel, PortType::digital, LOW, HIGH) { }

    turnOn() {
      Device::turnOn();
    }

    turnOff() {
      Device::turnOff();
    }

};
