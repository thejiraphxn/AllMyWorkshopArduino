#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

void setup() {
  myservo.attach(D3);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  
  myservo.write(0);                  // sets the servo position according to the scaled value
  delay(1500);                           // waits for the servo to get there
  myservo.write(180);                  // sets the servo position according to the scaled value
  delay(1500);                           // waits for the servo to get there
}
