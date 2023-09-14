

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  pinMode(9, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(9) == HIGH){
    digitalWrite(10, HIGH);
    delay(500);
    digitalWrite(10, LOW);
  }
  Serial.println(digitalRead(9));
}
