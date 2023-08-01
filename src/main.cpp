#include <Arduino.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <WiFi.h>

#include "config.h"

// Weather API
const String openweatherServer = "https://api.openweathermap.org/data/3.0/onecall";
const String serverPath = openweatherServer + "?lat=" + OPENWEATHER_LAT + "&lon=" + OPENWEATHER_LON + "&appid=" + OPENWEATHER_API_KEY;
JSONVar weatherData;

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORT);

  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

JSONVar httpGETRequest(String server) {
  HTTPClient http;
  http.begin(server.c_str());
  int httpResponseCode = http.GET();

  String payload = "{}";
  JSONVar data;

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  data = JSON.parse(payload);
  if (JSON.typeof(data) == "undefined") {
    Serial.println("Parsing input failed!");
    return data;
  }

  // myObject.keys() can be used to get an array of all the keys in the object
  JSONVar keys = data.keys();

  for (int i = 0; i < keys.length(); i++) {
    JSONVar value = data[keys[i]];
    Serial.print(keys[i]);
    Serial.print(" = ");
    Serial.println(value);
  }

  return data;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  // Connect WiFi
  initWiFi();
}

void loop() {
  // Send an HTTP POST request every x seconds/minutes
  if ((millis() - TIMESTAMP_WEATHER) > DELAY_WEATHER) {
    weatherData = httpGETRequest(serverPath);
    TIMESTAMP_WEATHER = millis();
  }
}