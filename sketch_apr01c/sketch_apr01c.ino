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
#define SPEEDFW 255
#define SPEEDBW 255

void setup()
{
  Serial.begin(9600);
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

  if (S1 == 1 && S2 == 1 && S3 == 0 && S4 == 1 && S5 == 1) {
    Backward();
  } else if (S1 == 1 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 1) {
    Forward();
  } else if ((S1 == 1 && S2 == 1 && S3 == 1 && S4 == 0 && S5 == 1) ||
             (S1 == 1 && S2 == 1 && S3 == 0 && S4 == 0 && S5 == 1) ||
             (S1 == 1 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 1)) {
    TurnLeft();
  } else if ((S1 == 0 && S2 == 0 && S3 == 0 && S4 == 1 && S5 == 1) ||
             (S1 == 0 && S2 == 0 && S3 == 1 && S4 == 1 && S5 == 1) ||
             (S1 == 0 && S2 == 1 && S3 == 1 && S4 == 1 && S5 == 1) ||
             (S1 == 0 && S2 == 1 && S3 == 0 && S4 == 1 && S5 == 1) ||
             (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 0)) {
    TurnRight();
  } else if (S1 == 0 && S2 == 0 && S3 == 0 && S4 == 0 && S5 == 1) {
    Stop();
  }
}

void TurnLeft(){
  Serial.println("Turn Left");
  analogWrite(ENA, 220);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(130);
}

void TurnRight(){
  Serial.println("Turn Right");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, 220);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(130);
}

void Stop() {
  Serial.println("Stop");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Forward() {
  Serial.println("Forward");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Backward() {
  Serial.println("Backward");
  analogWrite(ENA, SPEEDFW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENB, SPEEDFW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  delay(430);
}
