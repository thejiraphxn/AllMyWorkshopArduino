#include <ESP8266WiFi.h>
#include <time.h>
#include <StringSplitter.h>
const char* ssid = "TP-Link_1515";                  //ใส่ชื่อ SSID Wifi
const char* password = "43008791";          //ใส่รหัสผ่าน

int timezone = 7 * 3600;                    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                //กำหนดค่า Date Swing Time
String data;

void setup(){
 Serial.begin(115200);
 Serial.setDebugOutput(true);

  WiFi.mode(WIFI_STA);                                        //เชื่อมต่อ Wifi
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
     Serial.print(",");
     delay(1000);
   }
    configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
    Serial.println("\nWaiting for time");
    while (!time(nullptr)) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("");
}
       
void loop(){
  time_t now = time(nullptr);
  data = ctime(&now);
  Serial.println(data);
  StringSplitter *splitter = new StringSplitter(data, ' ', 5);
  int itemCount = splitter->getItemCount();
  
  for(int i = 0; i < itemCount; i++){
    String item = splitter->getItemAtIndex(i);
    Serial.println(item);
  }
  delay(1000);
  
}
