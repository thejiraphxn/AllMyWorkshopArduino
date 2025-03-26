#include "FirebaseESP8266.h"
#include "jsmn.h"
#include <FirebaseJson.h>
#include "FirebaseESP8266HTTPClient.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
//#define WIFI_SSID "TP-Link_1515"
//#define WIFI_PASSWORD "43008791"
#define WIFI_SSID "HUAWEI-3537-2.4G"
#define WIFI_PASSWORD "93293954"
#define SOLENOID D6 //SIGNAL IN1
#define ULTRASONIC A0
#define LOW_WATER_LEVEL 4
#define HIGH_WATER_LEVEL 22
#define IPADDRESS "http://192.168.0.139:8081"
#define FIREBASE_HOST "thejiraphxn-16-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "lBwpRVfm4RHywRafGRU1UPnnMUCwW0Mm0lPJPQ6q"
//char FIREBASE_HOST[100] = "thejiraphxn-16-default-rtdb.asia-southeast1.firebasedatabase.app";
//char FIREBASE_AUTH[100] = "lBwpRVfm4RHywRafGRU1UPnnMUCwW0Mm0lPJPQ6q";

//ประกาศ obj
FirebaseData firebaseData;
//FirebaseJson json;
HTTPClient http; //ทำ ให้ เรา ส่งขอมล ผ่าน http ได้
ESP8266WebServer server(80); //ให้บอร์ดดใช้ port 80
WiFiClient wifiClient; //ให้บอร์ดเป็็นตัวรับและตัวส่งข้อมูล ตัวเองเป็น client

String UsernameMember[2] = {"system", "admin"};
String PasswordMember[2] = {"system", "admin"};
String CurrentUserLogin = "";
String CurrentPassLogin = "";
String LoginErrorMsg, LoginSuccessMsg, AlertMsg = "Please Login";
String UsernameGetFromBox;
String PasswordGetFromBox;
String ControlButton;
String Valve_Status_Now = "Off";
float WaterLevel;
int Distance;
int timezone = 7 * 3600;                    //ตั้งค่า TimeZone ตามเวลาประเทศไทย
int dst = 0;                                //กำหนดค่า Date Swing Time

void Logout(){
  CurrentUserLogin = "";
  CurrentPassLogin = "";
  LoginErrorMsg = "", LoginSuccessMsg = "", AlertMsg = "Please Login";
  UsernameGetFromBox = "";
  PasswordGetFromBox = "";
  ControlButton = "";
  Valve_Status_Now = "Off";
  String Redirect = "<html><head><meta http-equiv=\"refresh\" content=\"2; URL=/\" /></head><body></body></html>";
  server.send(200, "text/html", Redirect);
  
}

void PageLanding(){
  if(LoginErrorMsg != ""){
    AlertMsg = "<div class=\"text-danger\">"+LoginErrorMsg+"</div>";
  } else if(LoginSuccessMsg != ""){
    AlertMsg = "<div class=\"text-success\">"+LoginSuccessMsg+"</div>";
  }
 

  String LoginPage = (String)"<!DOCTYPE html><html lang=\"en\"><head>"+
    "<meta charset=\"UTF-8\">"+
    "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"+
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"+
    "<link rel=\"stylesheet\" href=\""+IPADDRESS+"/iot-project/assets/css/style.css\">"+
    "<link rel=\"stylesheet\" href=\""+IPADDRESS+"/iot-project/assets/css/thai.css\">"+
    "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css\" />"+
    "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.9.1/font/bootstrap-icons.css\" />"+
    "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">"+
    "<title>Login</title>"+
    "</head>"+
    "<body class=\"font\">"+
    "<div class=\"container-fluid\">"+
        "<div style=\"padding-top: 6rem;\"></div>"+
        "<div class=\"row\">"+
            "<div class=\"col-md-4\"></div>"+
            "<div class=\"col-md-4 text-center\">"+
                "<h1 class=\"fw-blod text-center\">Smart Barrage</h1>"+
            "</div>"+
            "<div class=\"col-md-4\"></div>"+
        "</div>"+
        "<div class=\"container\">"+
            "<form class=\"row\" action=\"/\" method=\"get\">"+
                "<div class=\"col-md-4\"></div>"+
                "<div class=\"col-md-4 col frame\">"+
                    "<strong>"+
                        "<h4 class=\"text-center\" id=\"alert-message\">"+
                            AlertMsg+
                        "</h4></strong>"+
                    "<input style=\"margin-top: 1.5rem !important;\" type=\"text\" class=\"form-control\" name=\"username\" placeholder=\"Username\">"+
                    "<input style=\"margin-top: 1rem !important;\" type=\"password\" class=\"form-control\" name=\"password\" placeholder=\"Password\">"+
                    "<div class=\"row\" style=\"margin-top: 1rem !important;\">"+
                        "<div class=\"col-12\"><input type=\"submit\" class=\"btn btn-secondary col-12\" name=\"login\" value=\"Login\"></div>"+
                        "<div class=\"col-12\"><a class=\"btn btn-secondary col-12 mt-3\" href=\"/\">Refresh Path</a></div>"+
                "</div><div class=\"col-md-4\"></div></div>"+
              "</form>"+
            "</div>"+
            "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4\" crossorigin=\"anonymous\"></script></body></html>";


  if(server.hasArg("username") && server.hasArg("password")){
    LoginChecker();
  }  
 
    server.send(200, "text/html", LoginPage);
}


bool LoginChecker(){
  LoginErrorMsg = "";
  LoginSuccessMsg = "";
  UsernameGetFromBox = (String)server.arg("username");
  PasswordGetFromBox = (String)server.arg("password");
  Valve_Status_Now = (String)server.arg("status");
  int IDX = -2;
 

  if(UsernameGetFromBox == ""){
    LoginErrorMsg = "Please input your username.";
    return false;
  } else if(PasswordGetFromBox == ""){
    LoginErrorMsg = "Please input your password.";
    return false;
  } else{
    for(int i=0; i<2; i++){
      if((String)UsernameMember[i] == UsernameGetFromBox){
        IDX = -1;
        if((String)PasswordMember[i] == PasswordGetFromBox){
          IDX = i;
          break;
        }
      }
    }

    if(IDX != -1 && IDX != -2){
      CurrentUserLogin = (String)UsernameMember[IDX];
      CurrentPassLogin = (String)PasswordMember[IDX];
      LoginSuccessMsg = "Logged In";
      ControlButton = "<a class=\"btn btn-primary col-12 btn-lg\" href=\"/?username="+CurrentUserLogin+"&password="+CurrentPassLogin+"&status=On\">เปิดวาล์ว</a>";
      HandleControl();
      return true;
     
    } else if(IDX == -2){
      LoginErrorMsg = "Username was wrong !";
      return false;
     
    } else if(IDX == -1){
      LoginErrorMsg = "Password was wrong !";
      return false;
     
    } else{
      LoginErrorMsg = "Something wrong !";
      return false;
     
    }
  }
}

void HandleControl(){
  
  if((String)server.arg("status") == "On"){
    time_t TimeNow = time(nullptr);
    String StrTime = ctime(&TimeNow);
    
    ControlButton = "<a class=\"btn btn-danger col-12 btn-lg\" href=\"/?username="+CurrentUserLogin+"&password="+CurrentPassLogin+"&status=Off\">ปิดวาล์ว</a>";
    digitalWrite(SOLENOID, HIGH);
    Distance = round(analogRead(ULTRASONIC) / 10);
    if(Distance > 22){
      Distance = 22;
    }
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;
    
    Firebase.pushString(firebaseData, (String)StrTime+"/Valve_Status/", "On");
    Firebase.pushString(firebaseData, (String)StrTime+"/User_Control/", CurrentUserLogin);
    
  } else if((String)server.arg("status") == "Off"){
    time_t TimeNow = time(nullptr);
    String StrTime = ctime(&TimeNow);
    
    ControlButton = "<a class=\"btn btn-primary col-12 btn-lg\" href=\"/?username="+CurrentUserLogin+"&password="+CurrentPassLogin+"&status=On\">เปิดวาล์ว</a>";
    digitalWrite(SOLENOID, LOW);
    Distance = round(analogRead(ULTRASONIC) / 10);
    if(Distance > 22){
      Distance = 22;
    }
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;
    
    Firebase.pushString(firebaseData, (String)StrTime+"/Valve_Status/", "Off");
    Firebase.pushString(firebaseData, (String)StrTime+"/User_Control/", CurrentUserLogin);
  }
 
  String ControlPage = (String)"<!DOCTYPE html><html lang=\"en\"><head>"+
      "<meta http-equiv=\"refresh\" content=\"5;\">"+
      "<meta charset=\"UTF-8\">"+
      "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">"+
      "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"+
      "<link rel=\"stylesheet\" href=\""+IPADDRESS+"/iot-project/assets/css/thai.css\">"+
      "<link rel=\"stylesheet\" href=\""+IPADDRESS+"/iot-project/assets/css/style.css\">"+
      "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/animate.css/4.1.1/animate.min.css\" />"+
      "<link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap-icons@1.9.1/font/bootstrap-icons.css\" />"+
      "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-+0n0xVW2eSR5OomGNYDnhzAbDsOXxcvSN1TPprVMTNDbiYZCxYbOOl7+AMvyTG2x\" crossorigin=\"anonymous\">"+
      "<title>Login</title>"+
      "</head>"+
      "<body class=\"font\">"+
      "<div class=\"container-fluid pt-3\">"+
      "<div class=\"row ms-4 me-4 mb-0\">"+
                  "<div class=\"col-md-12\">"+
                      "<div class=\"row frame-content mt-4 me-1\">"+
                          "<div class=\"col-md-3\">"+
                              "<div class=\"row\">"+
                                  "<h3 class=\"text-left\">Control Panel</h3>"+
                                  "<h4 class=\"text-left\">แผงควบคุม</h4>"+
                                  "<div class=\"col-md-12 pt-3\">"+
                                      ControlButton+
                                  "</div>"+
                              "</div>"+
                          "</div>"+
                          "<div class=\"col-md-9\">"+
                              "<div class=\"row\">"+
                                  "<h3 class=\"text-left\">Dashboard</h3>"+
                                  "<h4 class=\"text-left\">รายงานข้อมูล</h4>"+
                                  "<div class=\"col-md-12 pt-2\">"+
                                      "<label class=\"text-left\">แสดงระดับน้ำ</label>"+
                                      "<div class=\"progress\" role=\"progressbar\" style=\"height: 30px\">"+
                                          "<div class=\"progress-bar\" style=\"width: "+WaterLevel+"%\" id=\"waterlevel\">"+WaterLevel+"%</div>"+
                                      "</div>"+
                                  "</div>"+
                                  "<div class=\"col-md-12 pt-3\">"+
                                      "<div class=\"frame-price text-center\">"+
                                          "วันและเวลาล่าสุดของการเปิดและปิดวาล์ว"+
                                      "</div>"+
                                  "</div>"+
                              "</div>"+
                          "</div>"+
                      "</div>"+
                  "</div>"+
              "</div>"+
      "</div>"+
      "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4\" crossorigin=\"anonymous\"></script></body></html>";
      server.send(200, "text/html", ControlPage);
}

void RealTimeSensorTrigger(){
  Distance = round(analogRead(ULTRASONIC) / 10);
  if(Distance <= 4){
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;
    String StatusData = "On "+(String)WaterLevel;
//    char JSONData[] = "{\"status\": \"OK\", \"data\": "+(String)Status+"}";
    digitalWrite(SOLENOID, LOW
    );
    server.send(200, "text/plain", StatusData);
  } else if(Distance > 4){
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;
    String StatusData = "Off "+(String)WaterLevel;
//    char JSONData[] = "{\"status\": \"OK\", \"data\": "+(String)Status+"}";
    digitalWrite(SOLENOID, HIGH);
    server.send(200, "text/plain", StatusData);
  }
 
}

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
  
  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");     //ดึงเวลาจาก Server
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  server.on("/", PageLanding); //Set path for request url/control action function RequestData
  server.on("/auto", RealTimeSensorTrigger); //Set path for request url/auto action function RealTimeSensorTrigger
  server.begin(); //Start server
  
  pinMode(D3, INPUT); //Set Pin for
  pinMode(ULTRASONIC, INPUT);
  pinMode(SOLENOID, OUTPUT);
 
}

void loop() {
  server.handleClient(); //รอรับ การร้องขอจาก client
}
