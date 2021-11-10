#include "../TemperatureSensor/TemperatureSensor.cpp"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.cpp"
#include "../DummySensor/DummySensor.cpp"
#include "../TemperatureSensor/enums.h"
#include "../SoilMoistureSensor/SoilMoistureSensor.cpp"
#include "../SensorGroup/SensorGroup.cpp"
#include "../Relay/Relay.cpp"
#include "../RealTimeClock/RealTimeClock.cpp"
#include "../VariableWattageFan/VariableWattageFan.cpp"
// #include "GrowController.h"


#include <RBDdimmer.h>

int targetTemp = 70;


namespace GrowController {

  int gotTemp = 0;


  class GrowController {
    public:
      GrowController() :
      fan(30),
      // tempSensor(28),
      // tempHumSensor(5),
      soilMoistureSensor(A3),
      // soilMoistureSensors([]),
      heater(3)
      {
        // fan.setPower(69);
        Serial.print("wtf");

      };


      setup() {
        // fan.setPower(65);
      };

      update() {
        // fan.setPower(95);

        // if (gotTemp == 10) {
          // tempSensor.update();
        //   Serial.print("tempSensor->getValue(): ");
        //   Serial.println(tempSensor.getValue());
        //   gotTemp = 0;
        // } else {
        //   gotTemp++;
        // }

      //
      soilMoistureSensor.update();
      Serial.print("soilMoistureSensor->getValue(): ");
      Serial.println(soilMoistureSensor.getValue());

      clock.update();


      dummyTempSensor.update();
      setFanPower(dummyTempSensor.getValue());

      // tempHumSensor.update();

      // Serial.print("VPD: ");
      // Serial.println(tempHumSensor.getVPD());

      // if (tempSensor.getValue() < 70) {
      //   heater.turnOn();
      // } else {
      //   heater.turnOff();
      // }

      // fan.setPower(70);

      // heater.turnOn();

      // Serial.print("huh");

      // for(int i = 100; i >= 1; i--) {
      //   this->fan.setPower(i);
      //   // di.setPower(i);
      //   Serial.print("Setting power to ");
      //   Serial.println(i);
      //   delay(100);
      // }

      };

    private:
      VariableWattageFan fan;
      // TemperatureSensor tempSensor;
      // TemperatureHumiditySensor tempHumSensor;
      SoilMoistureSensor soilMoistureSensor;
      // SensorGroup soilMoistureSensors;
      DummySensor dummyTempSensor;
      Relay heater;
      RealTimeClock clock;

      setFanPower(int temp) {
        int tempDifferential = temp - (targetTemp - 1); // only stop once we've passed the target temp

        Serial.print("dummyTempSensor->getValue(): ");
        Serial.println(temp);

        Serial.print("tempDifferential: ");
        Serial.println(tempDifferential);

        int fanPower = min(max(tempDifferential * 20, 0), 100);

        Serial.print("fan power: ");
        Serial.println(fanPower);
        fan.setPower(fanPower);

      }
  };

  // GrowController::GrowController() {
  //   // di.begin(NORMAL_MODE, ON_OFF_typedef::OFF);
  //   // Serial.println("running this");
  //   // di.setPower(69); // dude!
  //   // this->fan.begin(NORMAL_MODE, ON_OFF_typedef::ON);
  //   // fan.setPower(90);
  //   // delay(2000);
  //   // this->fan.setPower(90);
  // }
}
