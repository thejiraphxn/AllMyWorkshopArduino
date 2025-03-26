#define IN1 13
#define IN2 12
#define ENA 11
#define ENB 10
#define IN3 9
#define IN4 8
#define out5 7
#define out4 6
#define out3 5
#define out2 4
#define out1 3
#define Switch 2
#define SPEEDFW 255
#define SPEEDBW 255
int s = 1;
//#define SPEEDBW_2 0
//#define SPEEDBW_TURN 255

void setup()
{
  Serial.begin(9600);
  pinMode(Switch, INPUT);
  while(s == 1){
    s = digitalRead(Switch);
    Serial.println("Ready for start !");
    delay(100);
  }
  Serial.println("Start !");
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(out5, INPUT);
  pinMode(out4, INPUT);
  pinMode(out3, INPUT);
  pinMode(out2, INPUT);
  pinMode(out1, INPUT);
  
  
}
void loop() {
  int S1 = digitalRead(out1);
  int S2 = digitalRead(out2);
  int S3 = digitalRead(out3);
  int S4 = digitalRead(out4);
  int S5 = digitalRead(out5);
  
  if(S1==1 and S2==1 and S3==0 and S4==1 and S5==1)
  {
    Backward();
  }
  if(S1==1 and S2==1 and S3==1 and S4==1 and S5==1)
  {
    Forward();
  }
  else if(S1==1 and S2==1 and S3==1 and S4==0 and S5==1)
  {
    TurnLeft("IN");
  }
  else if(S1==1 and S2==1 and S3==1 and S4==0 and S5==0)
  {
    TurnLeft("IN");
  }
  else if(S1==1 and S2==1 and S3==1 and S4==1 and S5==0)
  {
    TurnLeft("OUT");
  }
  else if(S1==1 and S2==0 and S3==0 and S4==0 and S5==0)
  {
    TurnLeft("IN");
  }
  else if(S1==1 and S2==1 and S3==0 and S4==0 and S5==0)
  {
    TurnLeft("IN");
  }
  else if(S1==1 and S2==1 and S3==0 and S4==1 and S5==0)
  {
    TurnLeft("OUT");
  }
  else if(S1==1 and S2==0 and S3==1 and S4==1 and S5==1)
  {
    TurnRight("IN");
  }
  else if(S1==0 and S2==0 and S3==0 and S4==1 and S5==1)
  {
    TurnRight("IN");
  }
  else if(S1==0 and S2==0 and S3==1 and S4==1 and S5==1)
  {
    TurnRight("IN");
  }
  else if(S1==0 and S2==1 and S3==1 and S4==1 and S5==1)
  {
    TurnRight("OUT");
  }
  else if(S1==0 and S2==1 and S3==0 and S4==1 and S5==1)
  {
    TurnRight("IN");
  }
  else if(S1==0 and S2==0 and S3==0 and S4==0 and S5==1)
  {
    TurnRight("OUT");
  }
  else if(S1==0 and S2==0 and S3==0 and S4==0 and S5==0)
  {
    Stop();
  }

}
void TurnLeft(String sensor){
  if(sensor == "IN"){
    Serial.println("Turn Left");
    analogWrite(ENA, 200);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENB, SPEEDFW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(130);
  } 
  if(sensor == "OUT"){
    Serial.println("Turn Left");
    analogWrite(ENA, 160);
//    analogWrite(ENA, 160);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENB, SPEEDFW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(50);
//    delay(80);
  }
}

void TurnRight(String sensor){
  if(sensor == "IN"){
    Serial.println("Turn Right");
    analogWrite(ENA, SPEEDFW);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENB, 200);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(130);
  }
  if(sensor == "OUT"){
    Serial.println("Turn Right");
    analogWrite(ENA, SPEEDFW);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENB, 160);
//    analogWrite(ENB, 160);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(50);
    //    delay(80);
  }
}

//void TurnLeft(){
//  Serial.println("Turn Left");
//  analogWrite(ENA, SPEEDBW_2);
//  digitalWrite(IN1, LOW);
//  digitalWrite(IN2, HIGH);
//  analogWrite(ENB, SPEEDFW);
//  digitalWrite(IN3, HIGH);
//  digitalWrite(IN4, LOW);
//}
//
//void TurnRight(){
//  Serial.println("Turn Right");
//  analogWrite(ENA, SPEEDFW);
//  digitalWrite(IN1, HIGH);
//  digitalWrite(IN2, LOW);
//  analogWrite(ENB, SPEEDBW_2);
//  digitalWrite(IN3, LOW);
//  digitalWrite(IN4, HIGH);
//}


void Stop(){
  Serial.println("Stop");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}


void Forward(){
  Serial.println("Forward");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Backward(){
  Serial.println("Backward");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(390);
}
