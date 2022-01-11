#ifndef UTIL_H
#define UTIL_H
#include <Arduino.h>
#include <math.h>
#include <DS3232RTC.h>

namespace GrowController {
  bool isNan(int value) { return value != value; }
  bool isNan(uint8_t value) { return value != value; }
  bool isNan(uint16_t value) { return value != value; }
  bool isNan(float value) { return value != value; }
  bool isNan(long value) { return value != value; }
  bool isNan(double value) { return value != value; }

  float calculateVPSat(float temperatureC) {
    return 610.7 * pow(10, ((7.5 * temperatureC) / (237.3 + temperatureC)));
    // returns Pa
  }

  float calculateVPD(float temperatureC, float humidity) {
    long double dividend = ((7.5 * temperatureC) / (237.3 + temperatureC));
    return (calculateVPSat(temperatureC) * (1 - (humidity / 100.00))) / 1000;
    // returns kPa
  }

  float calculateHumidity(float temperatureC, float VPDkPa) {
    float VPSat = calculateVPSat(temperatureC);

    return 100 - (100 * ((VPDkPa * 1000) / VPSat));
  }

  float celsiusToF(float celsius) {
    return celsius * 1.8 + 32;
  }

  // time

  unsigned long timeToSeconds(tmElements_t time) {
    return (unsigned long)(time.Second)
      + ((unsigned long)time.Minute * 60)
      + ((unsigned long)time.Hour * 3600);
  }

  bool isBefore(tmElements_t time, tmElements_t comparator) {
    return timeToSeconds(time) < timeToSeconds(comparator);
  }

  bool isAfter(tmElements_t time, tmElements_t comparator) {
    return !isBefore(time, comparator);
  }

  bool isBetween(tmElements_t time, tmElements_t start, tmElements_t end) {
    return isAfter(time, start) && isBefore(time, end);
  }

  bool isDay(tmElements_t time, tmElements_t start, tmElements_t end) {
    if (isAfter(start, end)) { // turns off first
      return !isBetween(time, end, start);
    } else {
      return isBetween(time, start, end);
    }
  }

  void printTime(tmElements_t time, char name[]) {
    Serial.print(name);
    Serial.print(": ");
    Serial.print(time.Hour);
    Serial.print(":");
    Serial.print(time.Minute);
    Serial.print(":");
    Serial.println(time.Second);
  }

  void printTime(char name[], tmElements_t time) {
    printTime(time, name);
  }

  void printTime(tmElements_t time) {
    printTime(time, "time");
  }

  // multiplexer
  void tcaSelect(uint8_t i) {
    if (i > 7) return;

    Wire.beginTransmission(0x70);
    Wire.write(1 << i);
    Wire.endTransmission();
  }


  void scanI2CPortAddress() {
    Serial.println ();
    Serial.println ("I2C scanner. Scanning ...");
    byte count = 0;

    Wire.begin();
    for (byte i = 8; i < 120; i++)
    {
      Wire.beginTransmission (i);
      if (Wire.endTransmission () == 0)
        {
        Serial.print ("Found address: ");
        Serial.print (i, DEC);
        Serial.print (" (0x");
        Serial.print (i, HEX);
        Serial.println (")");
        count++;
        delay (1);  // maybe unneeded?
        } // end of good response
    } // end of for loop
    Serial.println ("Done.");
    Serial.print ("Found ");
    Serial.print (count, DEC);
    Serial.println (" device(s).");

    delay(5000); // wait 5 seconds for the next I2C scan
  }

}

#endif
