#include <Arduino.h>
// #include <ArduinoCrashMonitor.h>
#include "GrowController/GrowController.cpp"
#include "Types.h";
// #include "Wifi/Wifi.cpp"

using namespace GrowController;
// using namespace Watchdog;

// char ssid[] = ""; // WIFI SSID
// char password[] = ""; // WIFI PASSWORD

dayNightAirSettings_t tempSettings = {
  //      { target, min, max }
  { 22.34, 21, 24 }, // day
  { 20, 19, 22 } // night
};

dayNightAirSettings_t vpdSettings = {
  { 1.0, 0.9, 1.1 },
  { 0.9, 0.8, 1.0 }
};

wateringSettings_t wateringSettings = { 45, 3000, 30000 };

lightingSettings_t lightingSettings = {
  { 0, 0, 6 }, // 6:00
  { 0, 0, 0 } // 12:00
};


void setup() {
  // everything got stuffed into the setup function due to issues with a pointer to the growController instance
  Serial.available();
  Serial.begin(115200);

  GrowController::GrowController growController;

  growController.setTempSettings(tempSettings);
  growController.setVPDSettings(vpdSettings);
  growController.setWateringSettings(wateringSettings);

  // unsigned int currentLoop = 0;

  while (true) {
    // if (currentLoop == 0 || currentLoop % 100 == 0) {
    //   Serial.print("looping: ");
    //   Serial.println(currentLoop);
    // }
    // currentLoop++;
    growController.update();
    delay(1000);
  }

  return 0;
}

void loop() {};
