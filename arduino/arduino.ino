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
  { 24.5, 21, 27 }, // day
  { 20, 19, 22 } // night
};

dayNightAirSettings_t vpdSettings = {
  { 1.0, 0.9, 1.1 },
  { 1.1, 0.9, 1.2 }
};

wateringSettings_t wateringSettings = { 45, 3000, 30000 };

lightingSettings_t lightingSettings = {
  { 0, 0, 6 }, // on at 6:00 / 6 am
  { 0, 0, 0 } // off at 24:00 / midnight
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
