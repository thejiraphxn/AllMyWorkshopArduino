#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <StringSplitter.h>
#define WIFI_SSID "HUAWEI-3537-2.4G"
#define WIFI_PASSWORD "93293954"
#define SOLENOID D6 //SIGNAL IN1
#define ULTRASONIC A0
#define DISTANCE_TRIGGER_LEVEL 8 // คลาด เคลื่อน 3 cm
#define HIGH_WATER_LEVEL 22
#define IPADDRESS "http://192.168.8.135:8081"
#define FIREBASE_HOST "thejiraphxn-16-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "lBwpRVfm4RHywRafGRU1UPnnMUCwW0Mm0lPJPQ6q"

//ประกาศ obj
FirebaseData firebaseData;
FirebaseJson json;
HTTPClient http; //ทำให้เราส่งข้อมมูลผ่าน http ได้
ESP8266WebServer server(80); //ให้บอร์ดใช้ port 80
WiFiClient wifiClient; //ให้บอร์ดเป็นตัวรับและตัวส่งข้อมูล ตัวเองเป็น client Create TCP connection

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
  if(LoginErrorMsg != "") // ถ้ามี error
  {
    AlertMsg = "<div class=\"text-danger\">"+LoginErrorMsg+"</div>";//แสดงข้อความสีแดง
  } else if(LoginSuccessMsg != "") //ถ้าส่ง Logged In จาก LoginChecker มา
  {
    AlertMsg = "<div class=\"text-success\">"+LoginSuccessMsg+"</div>";//ข้อความสีเขียว
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
                        "<div class=\"col-12\"><a class=\"btn btn-secondary col-12 mt-3\" href=\"/logout\">Refresh Path</a></div>"+
                "</div><div class=\"col-md-4\"></div></div>"+
              "</form>"+
            "</div>"+
            "<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.0.1/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-gtEjrD/SeCtmISkJkNUaaKMoLD0//ElJ19smozuHV6z3Iehds+3Ulb9Bn9Plx0x4\" crossorigin=\"anonymous\"></script></body></html>";


    if(server.hasArg("username") && server.hasArg("password"))//ถ้ามี username and password (LoginChecker()= true)
    {
      LoginChecker();
    }  
 
    server.send(200, "text/html", LoginPage);//ถ้าเช็ค if แล้ว false ก็จะส่ง LoginPage วน
}


bool LoginChecker()
{
  LoginErrorMsg = "";
  LoginSuccessMsg = "";
  UsernameGetFromBox = (String)server.arg("username");//==paramiter username on url
  PasswordGetFromBox = (String)server.arg("password");//==paramiter password on url
//  Valve_Status_Now = (String)server.arg("status");//ถ้าไม่มีการกดปุ่ม = off แต่ถ้ามีการกดปุ่ม
  
  int IDX = -2;
 
  if(UsernameGetFromBox == "")
  {
    LoginErrorMsg = "Please input your username.";
    return false;
  } 
  else if(PasswordGetFromBox == "")
  {
    LoginErrorMsg = "Please input your password.";
    return false;
  } 
  else //ถ้ามีค่าใน UsernameGetFromBox และ PasswordGetFromBox
  {
    for(int i=0; i<2; i++)
    {
      if((String)UsernameMember[i] == UsernameGetFromBox) //UsernameMember[0,1]==UsernameGetFromBox {"system", "admin"} set IDX = -1
      {
        IDX = -1;
        if((String)PasswordMember[i] == PasswordGetFromBox) //PasswordMember[0,1]== PasswordGetFromBox {"system", "admin"} set IDX = 0,1
        {
          IDX = i;
          break;
        }
      }
    }

    if(IDX != -1 && IDX != -2)//IDX = 0,1 user/password ตรงกับ {"system", "admin"}
    {
      CurrentUserLogin = (String)UsernameMember[IDX];
      CurrentPassLogin = (String)PasswordMember[IDX];
      LoginSuccessMsg = "Logged In";
      ControlButton = "<a class=\"btn btn-primary col-12 btn-lg\" href=\"/?username="+CurrentUserLogin+"&password="+CurrentPassLogin+"&status=On\">เปิดวาล์ว</a>";
      HandleControl();
      return true;
     
    } 
    else if(IDX == -2)//UsernameMember[0,1]!= UsernameGetFromBox {"system", "admin"} IDX คงค่า = -2 ที่ set ไว้
    {
      LoginErrorMsg = "Username was wrong !";
      return false;
     
    } 
    else if(IDX == -1)//PasswordMember[0,1]!= PasswordGetFromBox {"system", "admin"} IDX = -1 จากการ check username
    {
      LoginErrorMsg = "Password was wrong !";
      return false;
     
    } 
    else//   IDX = -3,-4 ...
    {
      LoginErrorMsg = "Something wrong !";
      return false;
     
    }
  }
}

void HandleControl()//user/password ตรงกับ {"system", "admin"}
{
  String DateConverted;
  String TimeConverted;
  if((String)server.arg("status") == "On")
  {
    http.begin(wifiClient, (String)IPADDRESS+"/iot-project2/time-api.php");//ให้ http เริ่มการทำงาน แล้วเรียกไปที่ IPADDRESS+"/iot-project2/time-api.php
    int HTTP_CODE = http.GET();//ส่ง request แบบ get แล้วได้ค่า status 100-599 (200-299 ==OK)
    String StrTime = http.getString();//รับส่วน body ของเว็บที่ได้จาก echo php 2023-12-06_14:00:00
    StringSplitter *splitter = new StringSplitter((String)StrTime, '_', 2);//char splitter เป็น obj ของคลาส StringSplitter 
                                                                           //เพื่อตัด string ระหว่าง _ (date_time)
    int StringCount = splitter->getItemCount();//นับ Index ว่าตัดได้กี่ตัว
    DateConverted = (String)splitter->getItemAtIndex(0);//เอาข้อมูล at Index 0 (date)
    TimeConverted = (String)splitter->getItemAtIndex(1);//เอาข้อมูลat Index 1 (time)

    ControlButton = "<a class=\"btn btn-danger col-12 btn-lg\" href=\"/?username="+CurrentUserLogin+"&password="+CurrentPassLogin+"&status=Off\">ปิดวาล์ว</a>";
    digitalWrite(SOLENOID, LOW);//จ่าย High ให้ relay ไม่ทำงาน solenoid ปิด
    Distance = round(analogRead(ULTRASONIC) / 10);//round ปัดเศษ เป็นจำนวนเต็ม  
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;//(22-distance *100) / 22 == ระดับน้ำแบบ%

    json.add("TimeAction", TimeConverted);//เพิ่ม value จาก TimeConverted ใน key ชื่อ TimeAction ("TimeAction": "14:00:00(TimeConverted)"
    json.add("UserControl", CurrentUserLogin);
    json.add("ValveStatus", "On");
    json.add("WaterLevel", (int)HIGH_WATER_LEVEL - Distance);
    Firebase.pushJSON(firebaseData, DateConverted, json);//push แบบ json ไปที่ firebase 
    
  } 
  else if((String)server.arg("status") == "Off")//
  {
    http.begin(wifiClient, (String)IPADDRESS+"/iot-project2/time-api.php");//ให้ http เริ่มการทำงาน แล้วเรียกไปที่ IPADDRESS+"/iot-project2/time-api.php
    int HTTP_CODE = http.GET();//ส่ง request แบบ get แล้วได้ค่า status 100-599 (200-299 ==OK)
    String StrTime = http.getString();//รับส่วน body ของเว็บที่ได้จาก echo php 2023-12-06_14:00:00
    StringSplitter *splitter = new StringSplitter((String)StrTime, '_', 2);//char splitter เป็น obj ของคลาส StringSplitter 
                                                                           //เพื่อตัด string ระหว่าง _ (date_time)
    int StringCount = splitter->getItemCount();//นับ Index ว่าตัดได้กี่ตัว
    DateConverted = (String)splitter->getItemAtIndex(0);//get Item at Index 0 (date)
    TimeConverted = (String)splitter->getItemAtIndex(1);//get Item at Index 1 (time)
    
    ControlButton = "<a class=\"btn btn-primary col-12 btn-lg\" href=\"/?username="+CurrentUserLogin+"&password="+CurrentPassLogin+"&status=On\">เปิดวาล์ว</a>";
    digitalWrite(SOLENOID, HIGH);//จ่าย Low ให้ relay ทำงาน solenoid เปิด
    
    Distance = round(analogRead(ULTRASONIC) / 10);//round ปัดเศษ เป็นจำนวนเต็ม  
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;//(22-distance *100) / 22 == ระดับน้ำแบบ%
    
    json.add("TimeAction", TimeConverted);//เพิ่ม value จาก TimeConverted ใน key ชื่อ TimeAction ("TimeAction": "14:00:00(TimeConverted)"
    json.add("UserControl", CurrentUserLogin);
    json.add("ValveStatus", "Off");
    json.add("WaterLevel", (int)HIGH_WATER_LEVEL - Distance);
    Firebase.pushJSON(firebaseData, DateConverted, json);//push แบบ json ไปที่ firebase 

  }
 
  String ControlPage = (String)"<!DOCTYPE html><html lang=\"en\"><head>"+
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
                                      "<a class=\"btn btn-danger mt-2 col-12 btn-lg\" href=\"/logout\">ออกจากระบบ</a>"+
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

void RealTimeSensorTrigger()//auto
{
  Distance = round(analogRead(ULTRASONIC) / 10);//round ปัดเศษ เป็นจำนวนเต็ม  
  
  if(Distance <= DISTANCE_TRIGGER_LEVEL) //distance <=8
  {
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL; //(22-distance *100) / 22 == ระดับน้ำแบบ%
    String StatusData = "On "+(String)WaterLevel+" "+Distance;//
    String DateConverted;
    String TimeConverted;
    http.begin(wifiClient, (String)IPADDRESS+"/iot-project2/time-api.php");//ให้ http เริ่มการทำงาน แล้วเรียกไปที่ IPADDRESS+"/iot-project2/time-api.php
    int HTTP_CODE = http.GET();//ส่ง request แบบ get แล้วได้ค่า status 100-599 (200-299 ==OK)
    String StrTime = http.getString();//รับส่วน body ของเว็บที่ได้จาก echo php 2023-12-06_14:00:00
    StringSplitter *splitter = new StringSplitter((String)StrTime, '_', 2);//char splitter เป็น obj ของคลาส StringSplitter 
                                                                           //เพื่อตัด string ระหว่าง _ (date_time)
    digitalWrite(SOLENOID, LOW);//จ่าย High ให้ relay ไม่ทำงาน solenoid ปิด   
    
    int StringCount = splitter->getItemCount();//นับ Index ว่าตัดได้กี่ตัว
    DateConverted = (String)splitter->getItemAtIndex(0);//เอาข้อมูล at Index 0 (date)
    TimeConverted = (String)splitter->getItemAtIndex(1);//เอาข้อมูลat Index 1 (time)
    
    json.add("TimeAction", TimeConverted);//เพิ่ม value จาก TimeConverted ใน key ชื่อ TimeAction ("TimeAction": "14:00:00(TimeConverted)"
    json.add("UserControl", "System");
    json.add("ValveStatus", "On");
    json.add("WaterLevel", (int)HIGH_WATER_LEVEL - Distance);
    Firebase.pushJSON(firebaseData, DateConverted, json);//push แบบ json ไปที่ firebase 
    
    server.send(200, "text/plain", StatusData);//ส่งค่าไปหน้า server
    
  } 
  else if(Distance > DISTANCE_TRIGGER_LEVEL)//distance > 8
  {
    WaterLevel = ((HIGH_WATER_LEVEL - Distance) * 100) / HIGH_WATER_LEVEL;//(22-distance *100) / 22 == ระดับน้ำแบบ%
    String StatusData = "Off "+(String)WaterLevel+" "+Distance;
    String DateConverted;
    String TimeConverted;
    http.begin(wifiClient, (String)IPADDRESS+"/iot-project2/time-api.php");//ให้ http เริ่มการทำงาน แล้วเรียกไปที่ IPADDRESS+"/iot-project2/time-api.php
    int HTTP_CODE = http.GET();//ส่ง request แบบ get แล้วได้ค่า status 100-599 (200-299 ==OK)
    String StrTime = http.getString();//รับส่วน body ของเว็บที่ได้จาก echo php 2023-12-06_14:00:00
    StringSplitter *splitter = new StringSplitter((String)StrTime, '_', 2);//char splitter เป็น obj ของคลาส StringSplitter 
                                                                           //เพื่อตัด string ระหว่าง _ (date_time)
    digitalWrite(SOLENOID, HIGH);//จ่าย High ให้ relay ไม่ทำงาน solenoid ปิด   
                                                           
    int StringCount = splitter->getItemCount();//นับ Index ว่าตัดได้กี่ตัว
    DateConverted = (String)splitter->getItemAtIndex(0);//เอาข้อมูล at Index 0 (date)
    TimeConverted = (String)splitter->getItemAtIndex(1);//เอาข้อมูลat Index 1 (time)
    
    json.add("TimeAction", TimeConverted);//เพิ่ม value จาก TimeConverted ใน key ชื่อ TimeAction ("TimeAction": "14:00:00(TimeConverted)"
    json.add("UserControl", "System");
    json.add("ValveStatus", "Off");
    json.add("WaterLevel", (int)HIGH_WATER_LEVEL - Distance);
    Firebase.pushJSON(firebaseData, DateConverted, json);//push แบบ json ไปที่ firebase 
    
    server.send(200, "text/plain", StatusData);//ส่งค่าไปหน้า server
    
    
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
 
 
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);//ให้ เชื่อมต่อ wifi ใหม่ เมื่อ wifi disconnect
  Firebase.setReadTimeout(firebaseData, 1000 * 60);//Firebase จะรอให้ข้อมูลโหลดไม่เกิน 1 นาที (60 วินาที)
  Firebase.setwriteSizeLimit(firebaseData, "tiny"); //กำหนดให้เขียนข้อมูลขนาด tiny (1s)

  //คำสั่งในการกำหนดชื่อ function ที่ทำงานเมื่อ client เรียกมายัง URI(path หลังจาก .com  ที่กำหนดไว้
  server.on("/", PageLanding); //Set path for request url/control action function RequestData 
  server.on("/auto", RealTimeSensorTrigger); //Set path for request url/auto action function RealTimeSensorTrigger 
  server.on("/logout", Logout); //Set path for request url/logout action function Logout
  server.begin(); //Start server
 
  pinMode(ULTRASONIC, INPUT);
  pinMode(SOLENOID, OUTPUT);
 
}

void loop() {
  server.handleClient(); //รอรับ การร้องขอจาก client
}
