int ledPin = 3;
int analogPin = 0;
int val = 0;
int btnPin = 4;

void setup()
{
    // put your setup code here, to run once:
    pinMode(ledPin, OUTPUT);
    pinMode(btnPin, INPUT);
    Serial.begin(9600);
}

void loop()
{
    // put your main code here, to run repeatedly:
    val = analogRead(analogPin);
    analogWrite(ledPin, val / 4);
    Serial.println(val / 4);
    delay(100);
}