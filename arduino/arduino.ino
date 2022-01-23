#include <Arduino.h>
#include "GrowController/GrowController.cpp"
#include "Types.h"
#include <avr/wdt.h>
#include <ArduinoCrashMonitor.h>
// #include "Wifi/Wifi.cpp"

using namespace GrowController;
using namespace Watchdog;

// char ssid[] = ""; // WIFI SSID
// char password[] = ""; // WIFI PASSWORD

dayNightAirSettings_t tempSettings = {
  // { target, min, max }
  { 25.5, 22.8, 27 }, // day
  { 20, 19, 22 } // night
};

dayNightAirSettings_t vpdSettings = {
  { 1.0, 0.90, 1.05 },
  { 1.2, 1.1, 1.3 }
};

wateringSettings_t wateringSettings = { 50, 1000, 20000 };

lightingSettings_t lightingSettings = {
  { 0, 0, 8 }, // on at 8:00 / 8 am
  { 0, 0, 22 } // off at 22:00 / 10 pm
};


void setup() {
  // everything got stuffed into the setup function due to issues with a pointer to the growController instance
  Serial.begin(115200);
  CrashMonitor::begin();
  wdt_enable(WDTO_8S);


  GrowController::GrowController growController;
  Serial.println("initialized growController");

  growController.setTempSettings(tempSettings);
  growController.setVPDSettings(vpdSettings);
  growController.setWateringSettings(wateringSettings);
  growController.setLightingSettings(lightingSettings);

  int loopCount = 0;

  while (true) {
    wdt_reset();

    growController.update();

    Serial.print("loopCount: "); Serial.println(loopCount);
    loopCount++;
  }
}

void loop() {};
