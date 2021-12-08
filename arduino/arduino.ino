#include <Arduino.h>
// #include <ArduinoCrashMonitor.h>
#include "GrowController/GrowController.cpp"
// #include "Wifi/Wifi.cpp"

using namespace GrowController;
// using namespace Watchdog;

// char ssid[] = ""; // WIFI SSID
// char password[] = ""; // WIFI PASSWORD


void setup() {
  Serial.available();
  Serial.begin(115200);

  GrowController::GrowController growController;

  // CrashMonitor::begin();
  // // // Dump any crash reports to the serial port.
  // CrashMonitor::dump(Serial);
  // // // startWifi(*ssid, *password);
  // //
  // if (CrashMonitor::isFull()) {
  //   // We've stored as many crash reports as we can. Clear out the old ones so
  //   // we can have room for new ones.
  //   Serial.println(F("CrashMonitor report storage full."));
  //   Serial.println(F("Clearing crash reports from EEPROM ..."));
  //   CrashMonitor::clear();
  // }
  // //
  // // // Enable the watchdog timer with a timeout of 2 seconds.
  // CrashMonitor::enableWatchdog(Watchdog::CrashMonitor::Timeout_8s);

  // Serial.println("Starting");

  unsigned int currentLoop = 0;

  while (true) {
    if (currentLoop == 0 || currentLoop % 1 == 0) {
      Serial.print("looping: ");
      Serial.println(currentLoop);
    }
    currentLoop++;
    growController.update();
    delay(500);
  }

  return 0;
}

void loop() {};
