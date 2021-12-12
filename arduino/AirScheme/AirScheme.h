#ifndef AIR_SCHEME_H
#define AIR_SCHEME_H
#include "../Relay/Relay.h"
#include "../VariableWattageFan/VariableWattageFan.h"
#include "../Types.h"

namespace GrowController {

  class AirScheme {
    public:
      AirScheme(
        Relay *heater,
        Relay *humidifier,
        VariableWattageFan *fan
      ) {
        this->heater = heater;
        this->humidifier = humidifier;
        this->fan = fan;
      }

      update(
        float temp,
        float humidity,
        bool isDayTime,
        dayNightAirSettings_t tempSettings,
        dayNightAirSettings_t vpdSettings
      ) {
        float targetTemp = isDay
          ? tempSettings.day.target
          : tempSettings.night.target;

        airSettings_t currentVPDSettings = isDay
          ? vpdSettings.day
          : vpdSettings.night;

        airSettings_t currentTempSettings = isDay
          ? tempSettings.day
          : tempSettings.night;

        airSettings_t humiditySettings = {
          calculateHumidity(temp, currentVPDSettings.target),
          calculateHumidity(temp, currentVPDSettings.min),
          calculateHumidity(temp, currentVPDSettings.max)
        };

        this->handleFanControl(
          temp,
          currentTempSettings,
          this->fan
        );

        this->handleHumidifierControl(
          humidity,
          humiditySettings
        );

        this->handleHeaterControl(
          temp,
          humidity,
          currentTempSettings,
          humiditySettings
        );
      }

      handleHeaterControl(
        float temp,
        float humidity,
        airSettings_t tempSettings,
        airSettings_t humiditySettings
      ) {
        if (humidity > humiditySettings.max || temp < tempSettings.min) {
          this->heater->turnOn();
        } else {
          this->heater->turnOff();
        }
      }

      handleHumidifierControl(float humidity, airSettings_t humiditySettings) {
        if (humidity < humiditySettings.target) {
          this->humidifier->turnOn();
        } else {
          this->humidifier->turnOff();
        }
      }

      handleFanControl(
        float temp,
        airSettings_t tempSettings,
        VariableWattageFan *fan
      ) {
        Serial.print("temp: "); Serial.println(temp);
        Serial.print("tempSettings.target: "); Serial.println(tempSettings.target);

        float tempDifferential = temp - tempSettings.target; // only stop once we've passed the target temp

        Serial.print("tempDifferential: "); Serial.println(tempDifferential);

        // delay(1000);

        if (tempDifferential <= 0) {
          fan->setPower(0);
        } else {

          // double multiplier = 100 / pow(this->variance * 100.00, 3.9); // z = y/(x*100)^3.9
          // 1.5848931924611143e-06
          // 0.00000158489

          // Serial.print("multiplier: ");
          // Serial.println(multiplier);

          // long double multiplier = 100.00 / (long double)pow(100, this->multiplier);


          float adjustedDifferential =
            // (pow(tempDifferential * 100, 3.9) * this->multiplier ) + 1; // y = 0.0004(x^3.9) + 1
            (pow(tempDifferential * 100, 3.9) * this->multiplier) + 1; // y = 0.0004(x^3.9) + 1

          int fanPower = max(min(adjustedDifferential, 100), 0);

          Serial.print("fanPower: "); Serial.println(fanPower);

          // fan.setPower(25);
          fan->setPower(fanPower);
          // delay(500);
        }
      }


    private:
      Relay *heater;
      Relay *humidifier;
      VariableWattageFan *fan;
      float multiplier = 0.009;

  };

}

#endif
