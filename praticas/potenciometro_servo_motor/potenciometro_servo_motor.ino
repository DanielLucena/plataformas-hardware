#include <Servo.h>
Servo myservo;
int analogPin = 0;
int val = 0;

void setup()
{
    // put your setup code here, to run once:
    myservo.attach(9);
    Serial.begin(9600);
}

void loop()
{
    // put your main code here, to run repeatedly:

    //  myservo.write(180);
    //  delay(500);
    //  myservo.write(0);
    //  delay(500);
    //  myservo.write(90);
    //  delay(500);

    val = analogRead(analogPin);
    //  analogWrite(ledPin, val / 4);
    Serial.println(val);
    myservo.write(map(val, 0, 1023, 0, 180));
    delay(5);
}