void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP Touch Test");

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Touch: ");
  Serial.println(touchRead(13));
  delay(1000);
}
