#include <DS1307RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include "../Sensor/Sensor.h";
#include "../PortType.h";

namespace GrowController {
  const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  class RealTimeClock : Sensor {
    public:
      RealTimeClock()
        : Sensor(SDA, PortType::analog)
      {
        // if(this->init()) {
        //   Serial.println("SetTime success");
        //   // RTC.set(now());
        //
        //   if (RTC.read(this->tm)) {
        //     // this->update();
        //     RTC.setTime(this->tm);
        //   }
        // }
      }

      bool init() {
        int Hour, Min, Sec;

        if (sscanf(__TIME__, "%d:%d:%d", &Hour, &Min, &Sec) != 3) return false;

        this->tm.Hour = Hour;
        this->tm.Minute = Min;
        this->tm.Second = Sec;

        char Month[12];
        int Day, Year;
        uint8_t monthIndex;

        if (sscanf(__DATE__, "%s %d %d", Month, &Day, &Year) != 3) return false;

        for (monthIndex = 0; monthIndex < 12; monthIndex++) {
          if (strcmp(Month, monthName[monthIndex]) == 0) break;
        }

        if (monthIndex >= 12) return false;

        this->tm.Day = Day;
        this->tm.Month = monthIndex + 1;
        this->tm.Year = CalendarYrToTm(Year);

        return true;

      }

      update() {
        time_t time = RTC.get();
      }

      float getValue() {
        return this->currentValue;
      }

    private:
      float currentValue;
      temperatureUnit unit;
      tmElements_t tm;

  };

}
