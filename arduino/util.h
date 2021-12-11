#ifndef UTIL_H
#define UTIL_H
#include <Arduino.h>
#include <math.h>
#include <DS1307RTC.h>

namespace GrowController {
  float calculateVPSat(float temperature) {
    return (610.7 * pow(10,((7.5 * temperature) / (237.3 + temperature))))
      / 1000;
  }

  float calculateVPD(float temperature, float humidity) {
    long double dividend = ((7.5 * temperature) / (237.3 + temperature));
    return calculateVPSat(temperature) * (humidity / 100.00);
  }

  float calculateHumidity(float temperature, float VPD) {
    float VPSat = calculateVPSat(temperature);
    return VPD / VPSat * 100;
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
