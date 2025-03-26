#include <ESP8266WiFi.h>
#define WIFI_SSID "THEJIRAPHXN"
#define WIFI_PASSWORD "jiraphon123"

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print("Wait for connect\n");
  }
  Serial.print("Connected, IP address: ");
  Serial.print(WiFi.localIP());
}
void loop() {
//  Serial.print("Hello, World\n");
//  delay(500);

}
