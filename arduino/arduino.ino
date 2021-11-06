#include <OneWire.h>
#include <HttpClient.h>
#include <DallasTemperature.h>
#include <WiFiEspAT.h>
// #include "DeviceController/DeviceController.cpp";

char ssid[] = "Chez du Roxanne";
char password[] = "goofnugget";


WiFiClient wifiClient;
HttpClient httpClient(wifiClient);

OneWire oneWire(4);
DallasTemperature dTemp(&oneWire);

const int SOIL_MOISTURE = A3;

const char ON = LOW;
const char OFF = HIGH;

bool debug = true;

void log(String value) {
  if (debug == true) {
    Serial.println(value);
  }
}

void setup() {
  // put your setup code here, to run once:

  // DeviceController heater(4);

  pinMode(3, OUTPUT); // relay 1
  setRelay1(OFF);
  pinMode(4, INPUT); // temp sensor
  pinMode(SOIL_MOISTURE, INPUT); // soil moisture

  Serial.begin(9600);
  Serial1.begin(115200);  // start wifi chip
  Serial.println("new");

  // Serial.println("Starting wifi chip");
  //
  // WiFi.init(&Serial1); // start wifi
  //
  // // Connect to WiFi
  // if (WiFi.status() != WL_CONNECTED) {
  //   Serial.println("Connecting to WiFi...");
  //
  //   WiFi.begin(ssid, password); // open wifi connection
  //
  //   Serial.println("Connected to WiFi");
  // }
  //
  // httpClient.get("http://192.168.0.143", "/index.html");

  Serial.println("got index.html");

  // char response[];
  //
  while (httpClient.available()) {
    char c = httpClient.read();
    Serial.print(c);
  }

  Serial.print("\n");

}

void setRelay1(char value) {
  digitalWrite(3, value);
}

float getTemperature() {

  dTemp.requestTemperatures();
  return dTemp.getTempFByIndex(0);
}

float getSoilMoisture() {
  return analogRead(SOIL_MOISTURE);
}


void loop() {
  // put your main code here, to run repeatedly:

  float tempF = getTemperature();

  Serial.print("temp (f): ");
  Serial.println(tempF);

  Serial.print("soil moisture: ");
  Serial.println(getSoilMoisture());


  setRelay1(tempF < 70 ? ON : OFF);

  delay(1000);
}
