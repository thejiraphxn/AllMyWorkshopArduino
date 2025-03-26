#include <Servo.h>
Servo myServo;
int servoPin = 9;

const int trigPin = 10; // พิน Trig
const int echoPin = 11; // พิน Echo


void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.write(0);
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  
  float distance = duration * 0.034 / 2;

  if(distance <= 10){
    for (int angle = 0; angle <= 180; angle++) {
      myServo.write(angle);  // Set the servo position to the current angle
      delay(10);  // Wait for the servo to reach the position
    }
    delay(4000);
    for (int angle = 180; angle >= 0; angle--) {
      myServo.write(angle);  // Set the servo position to the current angle
      delay(10);  // Wait for the servo to reach the position
    }
  }

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(100);
}
