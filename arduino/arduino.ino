#include <OneWire.h>
#include <HttpClient.h>
#include <DallasTemperature.h>
#include <WiFiEspAT.h>

char ssid[] = "Chez du Roxanne";
char password[] = "goofnugget";

WiFiClient wifiClient;
HttpClient httpClient(wifiClient);

OneWire oneWire(4);
DallasTemperature dTemp(&oneWire);

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
  // Serial.begin(9600);

  pinMode(3, OUTPUT); // relay 1
  setRelay1(OFF);
  pinMode(4, INPUT); // temp sensor

  // Serial.begin(115200); // debugging
  // Serial3.begin(115200); // start wifi chip
  Serial.begin(9600);
  Serial1.begin(115200);
  Serial.println("Starting wifi chip");
  WiFi.init(&Serial1); // start wifi

  // Connect to WiFi
  Serial.println(WiFi.status() == WL_CONNECTED ? "connected" : "not connected");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting to WiFi...");

    WiFi.begin(ssid, password); // open wifi connection

    Serial.println("Connected to WiFi");
  }




  httpClient.get("http://192.168.0.143", "/index.html");

  Serial.println("got logo");

  // char response[];
  //
  while (httpClient.available()) {
    char c = httpClient.read();
    Serial.print(c);
  }

  Serial.print("\n");

  // Serial.print(response);

  Serial.println("after while");
}

void setRelay1(char value) {
  digitalWrite(3, value);
}

float getTemperature() {

  dTemp.requestTemperatures();
  return dTemp.getTempFByIndex(0);
}


void loop() {
  // put your main code here, to run repeatedly:
  // current = (current == HIGH ? LOW : HIGH);
  // setRelay1(current);
  // setRelay1(current);
  // Serial.println("wut");
  // setRelay1(ON);

  float tempF = getTemperature();
  Serial.println(tempF);

  setRelay1(tempF < 76 ? ON : OFF);

  delay(1000);
}
