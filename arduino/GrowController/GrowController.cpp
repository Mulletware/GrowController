#include <Adafruit_AHTX0.h>
#include <M2M_LM75A.h>
// #include "../TemperatureSensor/TemperatureSensor.cpp"
#include "../TemperatureHumiditySensor/TemperatureHumiditySensor.h"
#include "../DummySensor/DummySensor.cpp"
#include "../TemperatureSensor/enums.h"
#include "../SoilMoistureSensor/SoilMoistureSensor.h"
#include "../SoilMoistureSensorGroup/SoilMoistureSensorGroup.h"
#include "../Relay/Relay.h"
#include "../RealTimeClock/RealTimeClock.h"
#include "../VariableWattageFan/VariableWattageFan.h"
#include "../WateringValveDevice/WateringValveDevice.h"
#include "../WateringScheme/WateringScheme.h"
#include "../LightingScheme/LightingScheme.h"

#define DEFAULT_TARGET_TEMP 22.0

M2M_LM75A lm75a;

namespace GrowController {

  int smSensors[] = { A0, A1, A2, A3, A4 };

  class GrowController {

    float
      targetTemp = DEFAULT_TARGET_TEMP,
      targetVPD = 1.0,
      targetSoilMoisture = 89.0,
      multiplier = 4;
      // variance = 1.00; //variance should be calculated in a language that deals with large numbers more easily

    // Devies
    VariableWattageFan fan; // must come first
    WateringValveDevice wateringValve;
    Relay heater;
    Relay lights;

    // Sensors
    TemperatureHumiditySensor tempHumSensor;
    SoilMoistureSensorGroup soilMoistureSensors;

    // I2C Sensors
    RealTimeClock clock;
    DummySensor dummyTempSensor;

    // // bool isVPDControlled = true;

    // control schemes
    WateringScheme wateringScheme;
    LightingScheme lightingScheme;

    tmElements_t lightsOn;
    tmElements_t lightsOff;

    public:
      GrowController()
      :

      // output devices
      fan(4), // must come first
      wateringValve(48),
      heater(50),
      lights(52),

      // sensors
      soilMoistureSensors(smSensors, 5),
      dummyTempSensor(20.9, 21.00, 100),

      // i2c sensors
      clock(0),
      tempHumSensor(1),

      // control schemes
      wateringScheme(&wateringValve),
      lightingScheme(&lights)
      {

        this->lightsOn.Hour = 6;
        this->lightsOn.Minute = 0;
        this->lightsOn.Second = 0;

        this->lightsOff.Hour = 0;
        this->lightsOff.Minute = 0;
        this->lightsOff.Second = 0;

        // heater.turnOff();

      };


      setup() { };

      update() {
        Serial.print("time: ");
        Serial.print(clock.getTime().Hour);
        Serial.print(":");
        Serial.print(clock.getTime().Minute);
        Serial.print(":");
        Serial.println(clock.getTime().Second);

        tmElements_t now = clock.getTime();

        this->lightingScheme.update(now, this->lightsOn, this->lightsOff);

        this->tempHumSensor.update();
        // this->dummyTempSensor.update();

        // this->soilMoistureSensors.update();
        // int soilMoisture = this->soilMoistureSensors.getMovingAverage();
        // float temp = this->dummyTempSensor.getValue();

        float temp = this->tempHumSensor.getAverageTemp();
        Serial.print("temp: "); Serial.println(temp);

        float humidity = this->tempHumSensor.getAverageHumidity();
        Serial.print("humidity: "); Serial.println(humidity);

        float vpd = this->tempHumSensor.getAverageVPD();
        Serial.print("vpd: "); Serial.println(vpd);

        handleFanControl(temp);
      };

      setTargetTemp(float temp) {
        this->targetTemp = temp;
      }

      setTargetSoilMoisture(float targetSoilMoisture) {
        this->targetSoilMoisture = targetSoilMoisture;
      }

      setTargetVPD(float vpdkPa) {
        this->targetVPD = vpdkPa;
      }

    private:

      handleFanControl(float temp) {
        float tempDifferential = temp - targetTemp; // only stop once we've passed the target temp

        if (tempDifferential < 0) {
          fan.setPower(0);
        } else {

          // double multiplier = 100 / pow(this->variance * 100.00, 3.9); // z = y/(x*100)^3.9
          // 1.5848931924611143e-06
          // 0.00000158489

          // Serial.print("multiplier: ");
          // Serial.println(multiplier);


          float adjustedDifferential = tempDifferential > 0
            ? (pow(tempDifferential * 100, 3.9) * this->multiplier ) + 1 // y = 0.0004(x^3.9) + 1
            : 0;

          int fanPower = max(min(adjustedDifferential, 100), 0);

          Serial.print("fanPower: "); Serial.println(fanPower);

          fan.setPower(fanPower);
        }
      }
  };

}
