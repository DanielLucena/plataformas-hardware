//para infravermelho
#include <Arduino.h>
#define DISABLE_CODE_FOR_RECEIVER
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>

//para servo motor
#include <ESP32Servo.h>
#define PIN_CORTINA 32
//Servo myServo;
int cortinaState = 0;

//para adafruit
#include "config.h"
AdafruitIO_Feed *ac = io.feed("AC");
AdafruitIO_Feed *luminosidade = io.feed("luminosidade");
AdafruitIO_Feed *cortina = io.feed("cortina");

//para ldr
#define LDR_PIN 35
int currentLDR = 0;
int lastLDR = 0;
int LDRWarning = 700;

//para spiffs e ntp
#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include <FS.h>
#include "SPIFFS.h"

String stateStr;
String s;

struct tm timeinfo;

const char *ssid = "ssid";
const char *password = "password";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = -3 * 60 * 60;  // -3h*60min*60s = -10800s
const int daylightOffset_sec = 0;

const char *time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

int alarmeH = 11;
int alarmeM = 20;



#define LED_BUILTIN 2

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  //para infravermelho
  IrSender.begin(DISABLE_LED_FEEDBACK);

  //para servo motor
  pinMode(PIN_CORTINA, OUTPUT);
  // myServo.attach(PIN_SERVO);
  // myServo.write(0);

  //para ldr
  pinMode(LDR_PIN, INPUT);


  //para spiffs
  openFS();
  String ultimoRegistro = readFile("/led_state.txt");  // Lê do arquivo de logs e armazena, na variável, o último registro presente no arquivo.
  cortinaState = ultimoRegistro.toInt();                      // Converte o caracter inicial do registro ('0' ou '1') para Int.
  digitalWrite(PIN_CORTINA, cortinaState);

  //para ntp
  sntp_set_time_sync_notification_cb(timeavailable);
  sntp_servermode_dhcp(1);  // (optional)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // wait for serial monitor to open
  while (!Serial)
    ;

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  ac->onMessage(handleAC);
  cortina->onMessage(handleCortina);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  //ac->get();
  cortina->get();

  //para ntp
  while (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    delay(5000);
  }
}

void SwitchAC() {
  Serial.println("enviando infravermelho");
  IrSender.sendSamsung(0x707, 0x68, 0);
  delay(2000);
}
unsigned long lastTime = millis();

void loop() {
  //SwitchCortina();
  //delay(1000);
  // SwitchAC();


  // delay(5000);
  io.run();
  //Serial.println("IO RUN");

  if (millis() > lastTime + 30000) {
    currentLDR = analogRead(LDR_PIN);
    Serial.print("valor ldr: ");
    Serial.println(currentLDR);

    // save the current state to the 'digital' feed on adafruit io
    Serial.print("sending ldr -> ");
    Serial.println(currentLDR);
    luminosidade->save(currentLDR);
    if(currentLDR >= LDRWarning && !checkAlarme()){
      digitalWrite(PIN_CORTINA, HIGH);
      cortinaState = 1;
      cortina->save(cortinaState);
    }
    if(checkAlarme()){
      Serial.println("alarme tocou");
      SwitchAC();
      digitalWrite(PIN_CORTINA, LOW);
      cortinaState = 0;
      cortina->save(cortinaState);

    }
    printLocalTime();
    checkAlarme();

    // store last button state
    //lastLDR = currentLDR;
    lastTime = millis();
  }



  // delay(30000);
}

void handleAC(AdafruitIO_Data *data) {

  Serial.print("received <- ");

  // if(data->toPinLevel() == HIGH)
  //   Serial.println("HIGH");
  // else
  //   Serial.println("LOW");
  SwitchAC();
  //SwitchCortina();

  // digitalWrite(LED_PIN, data->toPinLevel());
}


void handleCortina(AdafruitIO_Data *data) {

  if (data->toPinLevel() == HIGH)
    cortinaState = 1;
  else
    cortinaState = 0;

  digitalWrite(PIN_CORTINA, data->toPinLevel());
  stateStr = String(data->toPinLevel()); // Convertendo de Int para String.
  writeFile(stateStr, "/led_state.txt");
}

void SwitchCortina() {
  cortinaState = !cortinaState;
  Serial.print("cortina: ");
  Serial.println(cortinaState);
  if (cortinaState) {
    digitalWrite(PIN_CORTINA, HIGH);
  } else {
    digitalWrite(PIN_CORTINA, LOW);
  }
  cortina->save(cortinaState);
}

// NTP functions

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%Y-%b-%d %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

//retorna true se ja passou da hora do alarme
bool checkAlarme(){
  int timeH = timeinfo.tm_hour;
  int timeM = timeinfo.tm_min;
  Serial.print("tm_hour");
  Serial.println(timeH);
  Serial.print("tm_min");
  Serial.println(timeM);
  if(timeH > alarmeH && timeM > alarmeM){
    return true;
  }
  false;

}

//SPIFFS functions

void writeFile(String state, String path) {  // Escreve conteúdo em um arquivo.
  File rFile = SPIFFS.open(path, "w");       // "w" para sobreescrever.
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  } else {
    rFile.print(state);
    Serial.print("Gravou: ");
    Serial.println(state);
  }
  rFile.close();
}

String readFile(String path) {
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");  //r+ leitura e escrita
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  } else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    while (rFile.position() < rFile.size()) {
      s = rFile.readStringUntil('\n');
      s.trim();
      Serial.println(s);
    }
    rFile.close();
    return s;
  }
}

void formatFile() {
  Serial.println("Formantando SPIFFS");
  SPIFFS.format();
  Serial.println("Formatou SPIFFS");
}

void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  } else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}