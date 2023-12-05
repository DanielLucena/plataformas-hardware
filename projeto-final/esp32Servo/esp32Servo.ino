#include <ESP32Servo.h>
#define PIN_SERVO 2
Servo myServo;

void setup() {
  // put your setup code here, to run once:
  myServo.attach(PIN_SERVO);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<180; i+=20){
    myServo.write(i);
    delay(500);
  }

  myServo.write(0);
    delay(1000);
}
