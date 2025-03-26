#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#define HOST_IPADDRESS "http://192.168.8.159/auto"
#define WIFI_SSID "HUAWEI-3537-2.4G"
#define WIFI_PASSWORD "93293954"

HTTPClient http;
WiFiClient wifiClient;

void setup(){
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected, IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println();

//  while(true){
    
    http.begin(wifiClient, (String)HOST_IPADDRESS);
    int httpCode = http.GET();
    String DataResponse;
    DataResponse = http.getString();
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        Serial.println("HTTP GET request successful");
        Serial.println("Response: " + DataResponse);
      }
    } else {
      Serial.println("HTTP GET request failed");
      Serial.println("Response: " + DataResponse);
      Serial.println(http.errorToString(httpCode).c_str());
    }
  
    
    http.end();
    delay(5000);
//  }
  
  
}

void loop() {
//  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
//  http.begin(wifiClient, (String)HOST_IPADDRESS);
//  int httpCode = http.GET();
//  String DataResponse;
//  DataResponse = http.getString();
//  if (httpCode > 0) {
//    if (httpCode == HTTP_CODE_OK) {
//      Serial.println("HTTP GET request successful");
//      Serial.println("Response: " + DataResponse);
//    }
//  } else {
//    Serial.println("HTTP GET request failed");
//  }
//
//  delay(5000);
}
