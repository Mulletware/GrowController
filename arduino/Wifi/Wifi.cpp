#ifndef WIFI
#define WIFI

#include <WiFiEspAT.h>
#include <HttpClient.h>
// #include "../HttpClient-2.2.0/HttpClient.h"

namespace GrowController {

  WiFiClient wifiClient;
  HttpClient httpClient(wifiClient);

  startWifi(char* ssid, char* password) {

    Serial1.begin(115200);  // start wifi chip

    Serial.println("Starting wifi chip");

    // WiFi.init(&Serial1); // start wifi

    // Connect to WiFi
    // if (WiFi.status() != WL_CONNECTED) {
    //   Serial.println("Connecting to WiFi...");
    //
    //   WiFi.begin(ssid, password); // open wifi connection
    //
    //   Serial.println("Connected to WiFi");
    // }
    //
    // httpClient.get("http://192.168.0.143", "/index.html");
    //
    // Serial.println("got index.html");

    // char response[];

    // while (httpClient.available()) {
    //   char c = httpClient.read();
    //   Serial.print(c);
    // }

  };

}

#endif
