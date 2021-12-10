#include <Adafruit_AHTX0.h>
#include <M2M_LM75A.h>
// #include "../TemperatureSensor/TemperatureSensor.cpp"
#include "../TemperatureHumiditySensorBME280/TemperatureHumiditySensorBME280.h"
#include "../TemperatureHumiditySensorSHT31/TemperatureHumiditySensorSHT31.h"
#include "../TemperatureHumiditySensorSI7021/TemperatureHumiditySensorSI7021.h"
#include "../TemperatureHumiditySensorAHT10/TemperatureHumiditySensorAHT10.h"
#include "../TemperatureHumiditySensorGroup/TemperatureHumiditySensorGroup.h"
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


namespace GrowController {

  typedef TemperatureHumiditySensorGroup<TemperatureHumiditySensorAHT10> AHTSensorGroup;

  int smSensors[] = { A0, A1, A2, A3, A4 };
  int ahtSensors[] = { 4, 5 };

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
    TemperatureHumiditySensorBME280 tempHumSensorBME280;
    TemperatureHumiditySensorSHT31 tempHumSensorSHT31;
    TemperatureHumiditySensorSI7021 tempHumSensorSI7021;
    TemperatureHumiditySensorAHT10 tempHumSensorAHT10;
    TemperatureHumiditySensorAHT10 tempHumSensorAHT10b;

    AHTSensorGroup tempHumSensorGroup;

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
      tempHumSensorBME280(1),
      tempHumSensorSHT31(2),
      tempHumSensorSI7021(3),
      tempHumSensorAHT10(4),
      tempHumSensorAHT10b(5),

      tempHumSensorGroup(ahtSensors, 2),

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
        tmElements_t now = clock.getTime();

        this->lightingScheme.update(now, this->lightsOn, this->lightsOff);

        Serial.print("time: ");
        Serial.print(now.Hour);
        Serial.print(":");
        Serial.print(now.Minute);
        Serial.print(":");
        Serial.println(now.Second);


        this->tempHumSensorBME280.update();
        this->tempHumSensorSHT31.update();
        this->tempHumSensorSI7021.update();
        this->tempHumSensorAHT10.update();
        this->tempHumSensorAHT10b.update();

        this->tempHumSensorGroup.update();

        // this->dummyTempSensor.update();

        // this->soilMoistureSensors.update();
        // int soilMoisture = this->soilMoistureSensors.getMovingAverage();
        // float temp = this->dummyTempSensor.getValue();

        float temp = this->tempHumSensorBME280.getAverageTemp();
        Serial.print("temp: "); Serial.println(temp);
        float temp2 = this->tempHumSensorSHT31.getAverageTemp();
        Serial.print("temp2: "); Serial.println(temp2);
        float temp3 = this->tempHumSensorSI7021.getAverageTemp();
        Serial.print("temp3: "); Serial.println(temp3);
        float temp4 = this->tempHumSensorAHT10.getAverageTemp();
        Serial.print("temp4: "); Serial.println(temp4);
        float temp5 = this->tempHumSensorAHT10b.getAverageTemp();
        Serial.print("temp5: "); Serial.println(temp5);
        float groupTemp = this->tempHumSensorGroup.getAverageTemp();
        Serial.print("groupTemp: "); Serial.println(groupTemp);

        float humidity = this->tempHumSensorBME280.getAverageHumidity();
        Serial.print("humidity: "); Serial.println(humidity);
        float humidity2 = this->tempHumSensorSHT31.getAverageHumidity();
        Serial.print("humidity2: "); Serial.println(humidity2);
        float humidity3 = this->tempHumSensorSI7021.getAverageHumidity();
        Serial.print("humidity3: "); Serial.println(humidity3);
        float humidity4 = this->tempHumSensorAHT10.getAverageHumidity();
        Serial.print("humidity4: "); Serial.println(humidity4);
        float humidity5 = this->tempHumSensorAHT10b.getAverageHumidity();
        Serial.print("humidity5: "); Serial.println(humidity5);
        float groupHumidity = this->tempHumSensorGroup.getAverageHumidity();
        Serial.print("groupHumidity: "); Serial.println(groupHumidity);

        float vpd = this->tempHumSensorBME280.getAverageVPD();
        Serial.print("vpd: "); Serial.println(vpd);
        float vpd2 = this->tempHumSensorSHT31.getAverageVPD();
        Serial.print("vpd2: "); Serial.println(vpd2);
        float vpd3 = this->tempHumSensorSI7021.getAverageVPD();
        Serial.print("vpd3: "); Serial.println(vpd3);
        float vpd4 = this->tempHumSensorAHT10.getAverageVPD();
        Serial.print("vpd4: "); Serial.println(vpd4);
        float vpd5 = this->tempHumSensorAHT10b.getAverageVPD();
        Serial.print("vpd5: "); Serial.println(vpd5);
        float groupVPD = this->tempHumSensorGroup.getAverageVPD();
        Serial.print("groupVPD: "); Serial.println(groupVPD);

        handleFanControl(groupTemp);
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
