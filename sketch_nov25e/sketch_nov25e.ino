#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define WIFI_SSID "HUAWEI-3537-2.4G"
#define WIFI_PASSWORD "93293954"
ESP8266WebServer server(80);
String[] USERNAME_MEMBER = {};

String KeepError;
String KeepSuccess;
String Button = (String)"<input type=\"submit\" class=\"mt-3 btn btn-primary col-12\" value=\"Unlock\">";
String LockInput;
String Alert;
String NowUserLogin;
String NowPassLogin;


void Lock(){
  KeepError = "";
  KeepSuccess = "";
  Button = (String)"<input type=\"submit\" class=\"mt-3 btn btn-primary col-12\" value=\"Unlock\">";
  LockInput = "";
  Alert = "";
  NowUserLogin = "";
  NowPassLogin = "";
  
  /////////////// Do something ////////////////////////
  // servo setting
  // close
  /////////////// Do something ////////////////////////
  String Redirect = "<html><head><meta http-equiv=\"refresh\" content=\"2; URL=/\" /></head><body></body></html>";
  server.send(200, "text/html", Redirect);
}

void LoginCheck(){
    String Username = server.arg("username");
    String Password = server.arg("password");
    NowUserLogin = (String)"value='"+server.arg("username")+"'";
    NowPassLogin = (String)"value='"+server.arg("password")+"'";
    if(Username == Default_Username){
      if(Password == Default_Password){
        /////////////// Do something ////////////////////////
        // servo setting
        // open 
        /////////////// Do something ////////////////////////
        KeepSuccess = "Unlocked now !";
        KeepError = "";
        Button = (String)"<a href=\"/lock\" class=\"mt-3 btn btn-danger col-12\">Lock</a>";
        LockInput = "disabled";
      } else{
        KeepError = "Your input username was wrong !";
        KeepSuccess = "";
        Button = (String)"<input type=\"submit\" class=\"mt-3 btn btn-primary col-12\" value=\"Unlock\">";
        LockInput = "";
        Lock();
      }
    } else{
      KeepError = "Your input password was wrong !";
      KeepSuccess = "";
      Button = (String)"<input type=\"submit\" class=\"mt-3 btn btn-primary col-12\" value=\"Unlock\">";
      LockInput = "";
      Lock();
    }
}

void LoginPage(){
  if(server.hasArg("username") && server.hasArg("password"))
  {
    LoginCheck();
  } 

  if(KeepSuccess != ""){
    Alert = (String)"<div class=\"text-center alert alert-primary\" role=\"alert\">"+KeepSuccess+"</div>";
  } else if(KeepError != ""){
    Alert = (String)"<div class=\"text-center alert alert-primary\" role=\"alert\">"+KeepError+"</div>";
  } else{
    Alert = "";
  }
  
  String LoginPageHtml = (String)"<!doctype html>"+
  "<html lang=\"en\">"+
  "<head>"+
    "<meta charset=\"utf-8\">"+
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"+
    "<title>Bootstrap demo</title>"+
    "<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-T3c6CoIi6uLrA9TneNEoa7RxnatzjcDSCmG1MXxSR1GAsXEV/Dwwykc2MPK8M2HN\" crossorigin=\"anonymous\">"+
  "</head>"+
  "<body>"+
    "<div class=\"container\"><div class=\"row\"><div class=\"col\"></div>"+
       "<div class=\"col-md-4\">"+
          Alert+
         "<form action=\"/\" method=\"get\"><label>Username</label><input " + LockInput + " " + NowUserLogin +" type=\"text\" class=\"form-control\" name=\"username\"><label>Password</label><input " + LockInput + " " + NowPassLogin +" type=\"password\" class=\"form-control\" name=\"password\">"+
            Button+
         "</form>"+
       "</div>"+
     "<div class=\"col\"></div></div></div>";
    LoginPageHtml += (String)"<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/js/bootstrap.bundle.min.js\" integrity=\"sha384-C6RzsynM9kWDrMNeT87bh95OGNyZPhcTNXj1NW7RuBCsyN/o0jlpcV8Qyq46cDfL\" crossorigin=\"anonymous\">"+
      "</script>"+
    "</body>"+
  "</html>";
  
  server.send(200, "text/html", LoginPageHtml);

  
}

void setup(){
  Serial.begin(115200);
  
  //////////////// Wait for connecting !
  
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
  
  /////////////// Do something ////////////////////////
  // servo setting
  // pinMode setting
  /////////////// Do something ////////////////////////

  /////////////// Url Path ///////////////
  server.on("/", LoginPage);
  server.on("/lock", Lock);
  server.begin();
}

void loop() {
  server.handleClient();
}
