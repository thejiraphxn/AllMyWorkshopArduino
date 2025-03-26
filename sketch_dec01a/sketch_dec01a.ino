void setup() {
  pinMode(A0, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0)/10);
  
}
