#include <Adafruit_AHTX0.h>
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
#include "../AirScheme/AirScheme.h"
#include "../Types.h"

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
      multiplier = 0.005;
      // variance = 1.00; //variance should be calculated in a language that deals with large numbers more easily

    // Devies
    VariableWattageFan fan; // must come first
    WateringValveDevice wateringValve;
    Relay heater;
    Relay lights;
    Relay humidifier;

    // Sensors
    TemperatureHumiditySensorSHT31 tempHumSensorSHT31;
    AHTSensorGroup tempHumSensorGroup;
    SoilMoistureSensorGroup soilMoistureSensors;

    // I2C Sensors
    RealTimeClock clock;
    DummySensor dummyTempSensor;

    // control schemes
    WateringScheme wateringScheme;
    LightingScheme lightingScheme;
    AirScheme airScheme;

    lightingSettings_t lightingSettings;

    public:
      GrowController()
      :

      // output devices
      fan(4), // must come first
      wateringValve(48),
      heater(50),
      lights(52),
      humidifier(46),

      // sensors
      soilMoistureSensors(smSensors, 5),
      dummyTempSensor(22.33, 22.48),

      // i2c sensors
      clock(0),
      tempHumSensorSHT31(2),
      tempHumSensorGroup(ahtSensors, 2),

      // control schemes
      wateringScheme(&wateringValve),
      lightingScheme(&lights),
      airScheme(&heater, &lights, &humidifier, &fan)
      { };


      setup() { };

      update() {
        tmElements_t now = clock.getTime();

        this->lightingScheme.update(now, this->lightingSettings);

        bool isDay = this->lightingScheme.isDay(now, this->lightingSettings);


        Serial.print("time: ");
        Serial.print(now.Hour);
        Serial.print(":");
        Serial.print(now.Minute);
        Serial.print(":");
        Serial.println(now.Second);


        this->tempHumSensorSHT31.update();
        this->tempHumSensorGroup.update();

        this->dummyTempSensor.update();


        float temp2 = this->tempHumSensorSHT31.getAverageTemp();
        Serial.print("temp2: "); Serial.println(temp2);

        float groupTemp = this->tempHumSensorGroup.getAverageTemp();
        Serial.print("groupTemp: "); Serial.println(groupTemp);

        float humidity2 = this->tempHumSensorSHT31.getAverageHumidity();
        Serial.print("humidity2: "); Serial.println(humidity2);

        float groupHumidity = this->tempHumSensorGroup.getAverageHumidity();
        Serial.print("groupHumidity: "); Serial.println(groupHumidity);

        float vpd2 = this->tempHumSensorSHT31.getAverageVPD();
        Serial.print("vpd2: "); Serial.println(vpd2);

        float groupVPD = this->tempHumSensorGroup.getAverageVPD();
        Serial.print("groupVPD: "); Serial.println(groupVPD);

        float humidity = calculateHumidity(groupTemp, groupVPD);

        float dummy = this->dummyTempSensor.getValue();
        Serial.print("dummy: "); Serial.println(dummy);

        // handleFanControl(groupTemp);
        handleFanControl(dummy);

        // airScheme.update();
      };

      setTempSettings(dayNightAirSettings_t settings) {
        this->tempSettings = settings;
      }

      setVPDSettings(dayNightAirSettings_t settings) {
        this->vpdSettings = settings;
      }

      setWateringSettings(wateringSettings_t wateringSettings) {
        this->wateringSettings = wateringSettings;
      }

      setLightingSettings(lightingSettings_t lightingSettings) {
        this->lightingSettings = lightingSettings;
      }

    private:

      handleFanControl(float temp) {
        float targetTemp = this->tempSettings.day.target;

        Serial.print("temp: "); Serial.println(temp);
        Serial.print("targetTemp: "); Serial.println(targetTemp);

        float tempDifferential = temp - targetTemp; // only stop once we've passed the target temp

        Serial.print("tempDifferential: "); Serial.println(tempDifferential);

        // delay(1000);

        if (tempDifferential <= 0) {
          fan.setPower(0);
        } else {

          // double multiplier = 100 / pow(this->variance * 100.00, 3.9); // z = y/(x*100)^3.9
          // 1.5848931924611143e-06
          // 0.00000158489

          // Serial.print("multiplier: ");
          // Serial.println(multiplier);

          // long double multiplier = 100.00 / (long double)pow(100, this->multiplier);


          float adjustedDifferential =
            // (pow(tempDifferential * 100, 3.9) * this->multiplier ) + 1; // y = 0.0004(x^3.9) + 1
            (pow(tempDifferential * 100, 3.9) * this->multiplier ) + 1; // y = 0.0004(x^3.9) + 1

          int fanPower = max(min(adjustedDifferential, 100), 0);

          Serial.print("fanPower: "); Serial.println(fanPower);

          // fan.setPower(25);
          fan.setPower(fanPower);
          // delay(500);
        }
      }

      dayNightAirSettings_t tempSettings;
      dayNightAirSettings_t vpdSettings;
      wateringSettings_t wateringSettings;
  };

}
