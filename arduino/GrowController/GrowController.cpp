#include <Adafruit_AHTX0.h>
#include "../TemperatureHumiditySensorBME280/TemperatureHumiditySensorBME280.h"
#include "../TemperatureHumiditySensorSHT31/TemperatureHumiditySensorSHT31.h"
#include "../TemperatureHumiditySensorSI7021/TemperatureHumiditySensorSI7021.h"
#include "../TemperatureHumiditySensorAHT10/TemperatureHumiditySensorAHT10.h"
#include "../TemperatureHumiditySensorGroup/TemperatureHumiditySensorGroup.h"
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

namespace GrowController
{

  typedef TemperatureHumiditySensorGroup<TemperatureHumiditySensorAHT10> AHTSensorGroup;
  typedef SensorGroup<SoilMoistureSensor> SoilMoistureSensorGroup;

  // int smSensors[] = { A0, A1, A2, A3, A4 };
  int ahtSensors[] = {2, 3, 4, 5, 6};

  class GrowController
  {

    // Devies
    VariableWattageFan fan; // must come first
    Relay heater;
    Relay lights;
    WateringValveDevice wateringValve;
    Switch humidifier;

    // Sensors
    // TemperatureHumiditySensorSHT31 tempHumSensorSHT31;
    AHTSensorGroup tempHumSensorGroup;
    // SoilMoistureSensorGroup soilMoistureSensors;

    // I2C Sensors
    RealTimeClock clock;

    // control schemes
    // WateringScheme wateringScheme;
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
          // soilMoistureSensors(smSensors, 5),

          // i2c sensors
          clock(0),
          // tempHumSensorSHT31(2),
          tempHumSensorGroup(ahtSensors, 5),

          // control schemes
          // wateringScheme(&wateringValve),
          lightingScheme(&lights),
          airScheme(&heater, &humidifier, &fan){};

    update()
    {
      tmElements_t now = clock.getTime();

      printTime(now);

      bool isDaytime = isDay(now, this->lightingSettings.on, this->lightingSettings.off);

      this->tempHumSensorGroup.update();
      float temp = this->tempHumSensorGroup.getAverageTemp();

      // soilMoistureSensors.update();
      // float soilMoisture = soilMoistureSensors.getMovingAverage();
      // wateringScheme.update(soilMoisture, this->wateringSettings.target);

      // Serial.print("soilMoisture: "); Serial.println(soilMoisture);
      Serial.print("this->wateringSettings.target: ");
      Serial.println(this->wateringSettings.target);

      Serial.print("temp: ");
      Serial.println(temp);
      Serial.print("this->tempSettings.day.target: ");
      Serial.println(this->tempSettings.day.target);
      Serial.print("this->tempSettings.day.min: ");
      Serial.println(this->tempSettings.day.min);
      Serial.print("this->tempSettings.day.max: ");
      Serial.println(this->tempSettings.day.max);

      if (isNan(temp))
      {
        // fatal error
        Serial.println("Error, no valid temperature found");
        delay(5000);
        this->lightingScheme.update(now, this->lightingSettings);
        this->fan.setPower(100);
        return;
      }

      Serial.print("temp: ");
      Serial.println(temp);

      Serial.println("    :::    " + String(isDaytime ? "Daytime" : "Nighttime"));
      if (isDaytime)
      {
        Serial.print("tempSettings.day.max: ");
        Serial.println(tempSettings.day.max);
      }
      else
      {
        Serial.print("tempSettings.night.max: ");
        Serial.println(tempSettings.night.max);
      }

      bool isHeatEmergency = temp > (isDaytime
                                         ? this->tempSettings.day.max
                                         : this->tempSettings.night.max);

      float humidity = this->tempHumSensorGroup.getAverageHumidity();
      Serial.println("humidity: " + String(humidity));
      delay(1000);
      float targetVPD = isDaytime ? this->vpdSettings.day.max : this->vpdSettings.night.max;
      Serial.println("target vpd: " + String(targetVPD));
      bool isHumidityEmergency = humidity < calculateHumidity(temp, targetVPD);
      Serial.println("target humidity: " + String(calculateHumidity(temp, targetVPD)));

      if (isHeatEmergency)
      {
        Serial.println("Temperature Emergency!");
        this->lights.turnOff();
        this->fan.setPower(100);
        delay(4000);
      }
      else if (isHumidityEmergency)
      {
        Serial.println("Humidity Emergency!");

        Serial.println("min acceptable humidity: " + String(calculateHumidity(temp, targetVPD)));
        Serial.println("current humidity: " + String(humidity));
        this->lights.turnOff();
        this->heater.turnOff();
        this->fan.setPower(0);
        this->humidifier.turnOn();
        delay(4000);
      }
      else
      {
        Serial.println("Nominal operation +1");
        this->lightingScheme.update(now, this->lightingSettings);

        // float humidity = this->tempHumSensorGroup.getAverageHumidity();
        float vpd = this->tempHumSensorGroup.getAverageVPD();

        airScheme.update(
            temp,
            humidity,
            isDaytime,
            this->tempSettings,
            this->vpdSettings);
      }
    };

    setTempSettings(dayNightAirSettings_t airSettings)
    {
      this->tempSettings = airSettings;
    }

    setVPDSettings(dayNightAirSettings_t airSettings)
    {
      this->vpdSettings = airSettings;
    }

    setWateringSettings(wateringSettings_t wateringSettings)
    {
      this->wateringSettings = wateringSettings;
      this->wateringValve.setWateringDuration(wateringSettings.onTime);
      this->wateringValve.setWateringRestrictedDuration(wateringSettings.offTime);
    }

    setLightingSettings(lightingSettings_t lightingSettings)
    {
      this->lightingSettings = lightingSettings;
    }

    setSettings(controllerSettings_t settings)
    {
      this->setTempSettings(settings.temp);
      this->setVPDSettings(settings.vpd);
      this->setWateringSettings(settings.watering);
      this->setLightingSettings(settings.lighting);
    }

  private:
    dayNightAirSettings_t tempSettings;
    dayNightAirSettings_t vpdSettings;
    wateringSettings_t wateringSettings;
  };

}
