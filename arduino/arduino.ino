#include <DS1307RTC.h>
#include <TimeLib.h>
#include <Wire.h>
#include "GrowController/GrowController.cpp"
#include "Wifi/Wifi.cpp"

using namespace GrowController;

char ssid[] = "Chez du Roxanne";
char password[] = "goofnugget";

GrowController::GrowController growController;

void setup() {
  Serial.begin(57600);
  // while (!Serial) {};
  startWifi(*ssid, *password);
  growController.setup();
}


void loop() {
  // put your main code here, to run repeatedly:
  growController.update();
  delay(1000);
}
