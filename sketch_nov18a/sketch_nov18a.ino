void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  
}

void loop() {
    int Ultrasonic_val = round(analogRead(A0) / 10);
    Serial.println(Ultrasonic_val);
    delay(500);


}
