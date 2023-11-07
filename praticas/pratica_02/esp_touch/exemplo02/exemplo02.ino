#define LED_PIN 13
#define BLINK_INTERVAL 250

int ledState = LOW;

unsigned long previousMillis = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= BLINK_INTERVAL){
    digitalWrite(LED_PIN,!digitalRead(LED_PIN));
    previousMillis = currentMillis;
  }
}
