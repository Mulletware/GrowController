#include "../TemperatureSensor/TemperatureSensor.cpp"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.cpp"
#include "../TemperatureSensor/enums.h"
#include "../SoilMoistureSensor/SoilMoistureSensor.cpp"
#include "../SensorGroup/SensorGroup.cpp"
#include "../Relay/Relay.cpp"
#include "../RealTimeClock/RealTimeClock.cpp"
#include "GrowController.h"

namespace GrowController {

  SoilMoistureSensor soilMoistureSensor(A3);
  SensorGroup soilMoistureSensors();
  Relay heater(3);
  TemperatureSensor tempSensor(4);
  TemperatureHumiditySensor tempHumSensor(5);
  RealTimeClock clock;

  GrowController::GrowController() {}

  GrowController::setup() {
    heater.turnOff();
  }

  GrowController::update() {
    tempSensor.update();
    Serial.print("tempSensor->getValue(): ");
    Serial.println(tempSensor.getValue());

    soilMoistureSensor.update();
    Serial.print("soilMoistureSensor->getValue(): ");
    Serial.println(soilMoistureSensor.getValue());

    clock.update();

    tempHumSensor.update();
    
    Serial.print("VPD: ");
    Serial.println(tempHumSensor.getVPD());

    if (tempSensor.getValue() < 70) {
      heater.turnOn();
    } else {
      heater.turnOff();
    }

  }
}
