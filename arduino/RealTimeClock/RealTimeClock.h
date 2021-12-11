#ifndef REALTIMECLOCK_h
#define REALTIMECLOCK_h

#include <DS1307RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include "../I2CSensor/I2CSensor.h";
#include "../Types.h";

namespace GrowController {
  const char *monthName[12] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
  };

  class RealTimeClock : I2CSensor {
    public:
      // RealTimeClock();

      RealTimeClock(int multiplexerAddress)
        : I2CSensor(multiplexerAddress)
      {
        I2CSensor::select();
        if(this->init() && RTC.write(this->tm)) { }
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

      tmElements_t getTime() {
        // this->update();
        // return this->currentValue;
        I2CSensor::select();
        RTC.read(this->tm);
        return this->tm;
      }

    private:
      float currentValue;
      temperatureUnit unit;
      tmElements_t tm;
  };

}

#endif
