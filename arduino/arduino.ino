#include <Arduino.h>
#include "GrowController/GrowController.cpp"
#include "Types.h"
#include <avr/wdt.h>
#include <ArduinoCrashMonitor.h>
// #include "Wifi/Wifi.cpp"
#include "Settings.h"

using namespace Watchdog;

// char ssid[] = ""; // WIFI SSID
// char password[] = ""; // WIFI PASSWORD

GrowController::GrowController *growController;

void setup()
{
  Serial.begin(115200);
  CrashMonitor::begin();
  wdt_enable(WDTO_8S);

  growController = new GrowController::GrowController();

  Serial.println("initialized growController");

  growController->setSettings(settings);
}

void loop()
{
  wdt_reset();

  growController->update();

  wdt_reset();
  delay(100);
}
