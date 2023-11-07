#include <SoftwareSerial.h>

SoftwareSerial ArduinoSlave(10, 11);
#define btnPin 7
String answer;
String msg, oldmsg;
int intVal = 0, oldIntVal = 0, btnState;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("ENTER Commands:");
  pinMode(btnPin, INPUT);
  ArduinoSlave.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Read sensor
  intVal = analogRead(A0);
  //Read answer from slave
  readSlavePort();

  if(digitalRead(btnPin) == HIGH){
    btnState = 1;
  }
  else{
    btnState = 0;
  }
  msg = String(intVal) + ',' + btnState;
  //Send data to slave
  if (oldmsg != msg) {
    
    Serial.print("Master sent : ");
    Serial.println(msg);
    //Serial.println(btnState);

    ArduinoSlave.print(msg);
    oldmsg = msg;
  }
  //Send answer to monitor
  if (answer != "") {
    Serial.print("Slave LED PWM value : ");
    Serial.println(answer);
    answer = "";
  }
  delay(1000);
}

void readSlavePort() {
  while (ArduinoSlave.available()) {
    delay(10);
    //Serial.println(ArduinoSlave.available());
    if (ArduinoSlave.available() > 0) {
      char c = ArduinoSlave.read(); // gets one byte from serial buffer
      answer += c; //makes the string readString
    }
  }
  ArduinoSlave.flush();
}
