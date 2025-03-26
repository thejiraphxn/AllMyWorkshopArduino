#include <ESP8266WiFi.h>
#define LED D2               
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  WiFi.begin("RSU-IOT", "1otRSU@6F00d");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);     Serial.print(".");
  }
  Serial.println("WiFi connected");
  server.begin();  // Starts the Server
  Serial.println("Server started");
  Serial.print("IP Address of network: "); // will IP address on Serial Monitor
  Serial.println(WiFi.localIP());
  Serial.print("Copy and paste the following URL: https://"); // Will print IP address in URL format
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop()
{
  WiFiClient client = server.available();
  if (!client)  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available()) {
    delay(1);
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  int value = LOW;
  if(request.indexOf("/LED=ON") != -1)  {
    digitalWrite(LED, HIGH); // Turn LED ON
    value = HIGH;
  }

    if(request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(LED, LOW); // Turn LED OFF
    value = LOW;
  }
//*------------------HTML Page Code---------------------*//
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print(" CONTROL LED: ");
  if(value == HIGH)       
    client.print("ON");
  else      
    client.print("OFF");
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>OFF</button></a><br />");
  client.println("</html>");
  delay(1);
} 
