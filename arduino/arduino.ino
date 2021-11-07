#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiEspAT.h>
#include "HttpClient-2.2.0/HttpClient.cpp";
#include "DeviceController/DeviceController.cpp";
#include "TemperatureSensor/TemperatureSensor.cpp";
#include "TemperatureSensor/enums.h";
#include "SoilMoistureSensor/SoilMoistureSensor.cpp";
#include "Relay/Relay.cpp";

char ssid[] = "Chez du Roxanne";
char password[] = "goofnugget";

WiFiClient wifiClient;
HttpClient httpClient(wifiClient);

OneWire oneWire(4);
DallasTemperature dTemp(&oneWire);


const int SOIL_MOISTURE = A3;

bool debug = true;

void log(String value) {
  if (debug == true) {
    Serial.println(value);
  }
}

TemperatureSensor tempSensor(4);
SoilMoistureSensor soilMoistureSensor(A3);

Relay heater(3);

void setup() {
  // put your setup code here, to run once:



  pinMode(3, OUTPUT); // relay 1
  setRelay1(HIGH);
  // pinMode(4, INPUT); // temp sensor

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

  while (httpClient.available()) {
    char c = httpClient.read();
    Serial.print(c);
  }

  Serial.print("\n");

}

void setRelay1(char value) {
  digitalWrite(3, value);
}

void loop() {
  // put your main code here, to run repeatedly:

  tempSensor.update();
  Serial.print("tempSensor->getValue(): ");
  Serial.println(tempSensor.getValue());

  soilMoistureSensor.update();
  Serial.print("soilMoistureSensor->getValue(): ");
  Serial.println(soilMoistureSensor.getValue());

  if (tempSensor.getValue() < 70) {
    heater.turnOn();
  } else {
    heater.turnOff();
  }

  delay(1000);
}
