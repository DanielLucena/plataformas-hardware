#include <SoftwareSerial.h>
SoftwareSerial ArduinoMaster(10, 11);
#define ledPin 12
String msg = "";
int ledVal = 0;
int intVal = 0, oldIntVal = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ArduinoMaster.begin(9600);
  pinMode(ledPin, OUTPUT);
//  digitalWrite(ledPin,HIGH);
//  delay(1000);
//  digitalWrite(ledPin,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  readMasterPort();
  convertMsgToCmd();
  // Send answer to master
  if (intVal != oldIntVal) {
    Serial.print("Master sent : ");
    Serial.println(intVal);

    ledVal = map(intVal, 0, 10, 0, 255);
    Serial.print("led value : ");
    Serial.println(ledVal);
    ArduinoMaster.print(ledVal);
    analogWrite(ledPin, ledVal);
    oldIntVal = intVal;
  }
}

void readMasterPort() {
  while (ArduinoMaster.available()) {
    delay(10);
    if (ArduinoMaster.available() > 0) {
      char c = ArduinoMaster.read(); // gets one byte from serial buffer
      msg += c; //makes the string readString
    }
  }
  ArduinoMaster.flush();
}

void convertMsgToCmd() {
  if (msg.length() > 0) {
    Serial.print("message length : ");
    Serial.println(msg.length());

    char carray1[6]; //magic needed to convert string to a number
    msg.toCharArray(carray1, sizeof(carray1));
    intVal = atoi(carray1);
    msg = "";
  }
}
