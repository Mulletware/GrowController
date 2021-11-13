#include "GrowController/GrowController.cpp"
// #include "Wifi/Wifi.cpp"

using namespace GrowController;

char ssid[] = "Chez du Roxanne";
char password[] = "goofnugget";


GrowController::GrowController *growController; // created as pointer so it can be "newed" later, check this assumption

void setup() {
  // startWifi(*ssid, *password);

  int noSerial = 0;
  while (!Serial) { };
  Serial.begin(9600);

  growController = new GrowController::GrowController();
  growController->setup();
  growController->setTargetTemp(70);
  // no serial.print statements can go here
}


int currentLoop = 0;

void loop() {

  // put your main code here, to run repeatedly:
  Serial.print("looping ");
  Serial.println(currentLoop++);
  growController->update();


  delay(0);

}
