#include <Arduino.h>
#include <IRremote.h>



#define RECEIVER_PIN D5
IRrecv receiver(RECEIVER_PIN);
decode_results results;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
    receiver.enableIRIn();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (receiver.decode(&results)) {
    Serial.println(results.value, HEX);

    if (results.value == 0x97680707) {
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    receiver.resume();
  }
}
