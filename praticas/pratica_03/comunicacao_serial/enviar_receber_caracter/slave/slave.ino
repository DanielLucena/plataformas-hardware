#include <SoftwareSerial.h>
SoftwareSerial ArduinoMaster(10,11);
char cmd="";
char old_cmd;

void setup() {
  // put your setup code here, to run once:
  ArduinoMaster.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  old_cmd=cmd;
  // Read data from master
  if (ArduinoMaster.available()) {
    cmd=ArduinoMaster.read();
  }
  // Send answer to master
  if(cmd!=old_cmd) {
    ArduinoMaster.write(cmd);
  }
}
