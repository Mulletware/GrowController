#include "GrowController/GrowController.cpp"
// #include "Wifi/Wifi.cpp"

#include <RBDdimmer.h>

dimmerLamp dimb(30);

using namespace GrowController;

char ssid[] = "Chez du Roxanne";
char password[] = "goofnugget";


GrowController::GrowController *growController;

void setup() {
  // startWifi(*ssid, *password);

  int noSerial = 0;
  while (!Serial) { };
  Serial.begin(9600);

  growController = new GrowController::GrowController();
  growController->setup();
  // no serial.print statements can go here
}


int currentLoop = 0;

void loop() {

  Serial.print("looping ");
  Serial.println(currentLoop++);
  // put your main code here, to run repeatedly:
  growController->update();
  // dimmer.setPower(50); // name.setPower(0%-100%)


  delay(500);

}
