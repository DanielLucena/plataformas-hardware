#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include <FS.h>
#include "SPIFFS.h"

String stateStr;
String s;

int ledPin = 2;
int btnPin = 4;
int btnFormat = 5;
int state = 0;

const char* ssid       = "ssid";
const char* password   = "password";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long  gmtOffset_sec = -3 * 60 * 60;   // -3h*60min*60s = -10800s
const int   daylightOffset_sec = 0;

const char* time_zone = "CET-1CEST,M3.5.0,M10.5.0/3";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%Y-%b-%d %H:%M:%S");
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
}

//SPIFFS functions

void writeFile(String state, String time, String path) { // Escreve conteúdo em um arquivo.
  File rFile = SPIFFS.open(path, "a"); // "a" para anexar.
  int num = 100; // Número máximo de registros.
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    if (rFile.size() / 23 + 1 > num) { // Ao passar de 'num' registros, apaga os anteriores e grava o novo registro.
      rFile.close();
      rFile = SPIFFS.open(path, "w"); // "w" para apagar os dados anteriores e escrever apenas o novo registro.
      Serial.println("Registros anteriores descartados. Iniciando nova série de registros.");
    }
    struct tm timeinfo;
    rFile.print(state);
    rFile.print(" ");
    rFile.println(time);
    Serial.print("Gravou: ");
    Serial.print(state + " ");
    Serial.println(time);
    Serial.print("Quantidade de registros: ");
    Serial.println(rFile.size() / 23 + 1);
  }
  rFile.close();
}

String readFile(String path) {
  Serial.println("Read file");
  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("----------Lendo arquivo ");
    Serial.print(path);
    Serial.println("  ---------");
    Serial.print("Quantidade de registros: ");
    Serial.println(rFile.size() / 23);
    while (rFile.position() < rFile.size())
    {
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
  }
  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}

void setup()
{
  Serial.begin(115200);

  // set notification call-back function
  sntp_set_time_sync_notification_cb( timeavailable );

  /**
   * NTP server address could be aquired via DHCP,
   *
   * NOTE: This call should be made BEFORE esp32 aquires IP address via DHCP,
   * otherwise SNTP option 42 would be rejected by default.
   * NOTE: configTime() function call if made AFTER DHCP-client run
   * will OVERRIDE aquired NTP server address
   */
  sntp_servermode_dhcp(1);    // (optional)

  /**
   * This will set configured ntp servers and constant TimeZone/daylightOffset
   * should be OK if your time zone does not need to adjust daylightOffset twice a year,
   * in such a case time adjustment won't be handled automagicaly.
   */
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  /**
   * A more convenient approach to handle TimeZones with daylightOffset 
   * would be to specify a environmnet variable with TimeZone definition including daylight adjustmnet rules.
   * A list of rules for your zone could be obtained from https://github.com/esp8266/Arduino/blob/master/cores/esp8266/TZ.h
   */
  //configTzTime(time_zone, ntpServer1, ntpServer2);

  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");

  openFS();
  String ultimoRegistro = readFile("/logs.txt"); // Lê do arquivo de logs e armazena, na variável, o último registro presente no arquivo.
  state = ultimoRegistro.toInt(); // Converte o caracter inicial do registro ('0' ou '1') para Int.
  pinMode(ledPin, OUTPUT);
  pinMode(btnPin, INPUT);
  pinMode(btnFormat, INPUT);
  digitalWrite(ledPin, state);
}

void loop()
{
  struct tm timeinfo;
  char date[20];
  String dateStr;

  while(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    delay(5000);
  }

  if(!digitalRead(btnPin)) {  // Ao apertar o botão, altera o estado do led e salva um registro com o novo estado e a hora da modificação no arquivo "logs.txt".
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", &timeinfo);
    dateStr = String(date);
    state = !state;
    digitalWrite(ledPin, state);
    stateStr = String(state); // Convertendo de Int para String.
    writeFile(stateStr, dateStr, "/logs.txt");
    while(!digitalRead(btnPin)){
      delay(200);
    }
  }

  if(!digitalRead(btnFormat)) {
    formatFile();
    while(!digitalRead(btnFormat)){
      delay(200);
    }
  }
}