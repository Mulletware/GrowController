#include <Adafruit_AHTX0.h>
#include "../TemperatureHumiditySensorBME280/TemperatureHumiditySensorBME280.h"
#include "../TemperatureHumiditySensorSHT31/TemperatureHumiditySensorSHT31.h"
#include "../TemperatureHumiditySensorSI7021/TemperatureHumiditySensorSI7021.h"
#include "../TemperatureHumiditySensorAHT10/TemperatureHumiditySensorAHT10.h"
#include "../TemperatureHumiditySensorGroup/TemperatureHumiditySensorGroup.h"
#include "../DummySensor/DummySensor.cpp"
#include "../TemperatureSensor/enums.h"
#include "../SoilMoistureSensor/SoilMoistureSensor.h"
#include "../SensorGroup/SensorGroup.h"
#include "../Relay/Relay.h"
#include "../RealTimeClock/RealTimeClock.h"
#include "../VariableWattageFan/VariableWattageFan.h"
#include "../WateringValveDevice/WateringValveDevice.h"
#include "../Switch/Switch.h"
#include "../WateringScheme/WateringScheme.h"
#include "../LightingScheme/LightingScheme.h"
#include "../AirScheme/AirScheme.h"
#include "../Types.h"

#define DEFAULT_TARGET_TEMP 22.0


namespace GrowController {

  typedef TemperatureHumiditySensorGroup<TemperatureHumiditySensorAHT10> AHTSensorGroup;
  typedef SensorGroup<SoilMoistureSensor> SoilMoistureSensorGroup;

  int smSensors[] = { A0, A1, A2, A3, A4 };
  int ahtSensors[] = { 1, 2, 3, 4, 5 };

  class GrowController {

    float
      targetTemp = DEFAULT_TARGET_TEMP,
      targetVPD = 1.0,
      targetSoilMoisture = 89.0;
      // variance = 1.00; //variance should be calculated in a language that deals with large numbers more easily

    // Devies
    VariableWattageFan fan; // must come first
    Relay heater;
    Relay lights;
    WateringValveDevice wateringValve;
    Switch humidifier;

    // Sensors
    // TemperatureHumiditySensorSHT31 tempHumSensorSHT31;
    AHTSensorGroup tempHumSensorGroup;
    SoilMoistureSensorGroup soilMoistureSensors;

    // I2C Sensors
    RealTimeClock clock;
    // DummySensor dummyTempSensor;

    // control schemes
    WateringScheme wateringScheme;
    LightingScheme lightingScheme;
    AirScheme airScheme;

    lightingSettings_t lightingSettings;

    public:
      GrowController()
      :

      // output devices
      fan(3), // must come first
      lights(32),
      heater(34),
      wateringValve(40),
      humidifier(42),

      // sensors
      soilMoistureSensors(smSensors, 5),
      // dummyTempSensor(23.9, 24.1),

      // i2c sensors
      clock(0),
      // tempHumSensorSHT31(2),
      tempHumSensorGroup(ahtSensors, 5),

      // control schemes
      wateringScheme(&wateringValve),
      lightingScheme(&lights),
      airScheme(&heater, &humidifier, &fan)
      {
      };

      update() {
        tmElements_t now = clock.getTime();

        Serial.print(now.Hour); Serial.print(":");
        Serial.print(now.Minute); Serial.print(":");
        Serial.println(now.Second);

        bool isDaytime = isDay(now, this->lightingSettings.on, this->lightingSettings.off);

        this->tempHumSensorGroup.update();
        float temp = this->tempHumSensorGroup.getAverageTemp();

        Serial.print("temp: "); Serial.println(temp);
        Serial.print("this->tempSettings.day.max: "); Serial.println(this->tempSettings.day.max);

        bool isHeatEmergency = temp > (
          isDaytime
            ? this->tempSettings.day.max
            : this->tempSettings.night.max
        );

        if (isHeatEmergency) {
          Serial.println("Emergency!");
          this->lights.turnOff();
          this->fan.setPower(100);
        } else {
          this->lightingScheme.update(now, this->lightingSettings);

          float humidity = this->tempHumSensorGroup.getAverageHumidity();
          float vpd = this->tempHumSensorGroup.getAverageVPD();

          // this->dummyTempSensor.update();
          // float dummyTemp = this->dummyTempSensor.getValue();

          airScheme.update(
            temp,
            humidity,
            isDay,
            this->tempSettings,
            this->vpdSettings
          );

        }
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

      dayNightAirSettings_t tempSettings;
      dayNightAirSettings_t vpdSettings;
      wateringSettings_t wateringSettings;
  };

}
