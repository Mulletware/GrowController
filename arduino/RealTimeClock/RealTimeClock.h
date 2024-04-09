#ifndef REALTIMECLOCK_H
#define REALTIMECLOCK_H

#include <DS3232RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include <avr/wdt.h>
#include "../I2CSensor/I2CSensor.h";
#include "../Types.h";

namespace GrowController
{
  const char *monthName[12] = {
      "Jan", "Feb", "Mar", "Apr", "May", "Jun",
      "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

  class RealTimeClock : I2CSensor
  {
  public:
    RealTimeClock(int multiplexerAddress, bool shouldSetTime = false)
        : I2CSensor(multiplexerAddress)
    {
      I2CSensor::select();

      if (shouldSetTime)
      {
        this->setTime();
      }
    }

    setTime()
    {
      int Hour, Min, Sec;

      if (sscanf(__TIME__, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
        return false;

      char Month[12];
      int Day, Year;
      uint8_t monthIndex;

      if (sscanf(__DATE__, "%s %d %d", Month, &Day, &Year) != 3)
        return false;

      this->tm.Hour = Hour;
      this->tm.Minute = Min;
      this->tm.Second = Sec;

      for (monthIndex = 0; monthIndex < 12; monthIndex++)
      {
        if (strcmp(Month, monthName[monthIndex]) == 0)
          break;
      }

      if (monthIndex >= 12)
        return false;

      this->tm.Day = Day;
      this->tm.Month = monthIndex + 1;
      this->tm.Year = CalendarYrToTm(Year);

      Serial.println("Setting time");
      RTC.set(makeTime(this->tm));
      delay(6000);
    }

    tmElements_t getTime()
    {
      I2CSensor::select();
      RTC.read(this->tm);
      // Serial.println(String(this->tm.Hour) + ":" + String(this->tm.Minute) + ":" + String(this->tm.Second));
      // delay(2000);
      delay(200);
      return this->tm;
    }

  private:
    float currentValue;
    temperatureUnit unit;
    tmElements_t tm;
  };

}

#endif
