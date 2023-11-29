//Programa: WifiManager com ESP8266 e ESP32
//Autor: Arduino e Cia

//Carrega as biblioteca de acordo com a placa utilizada

#if defined(ESP8266)
//Bibliotecas ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#else
//Bibliotecas ESP32
#include <WiFi.h>
#include <WebServer.h>
#endif

//Bibliotecas comuns
#include <DNSServer.h>
#include <WiFiManager.h>

//Define o pino para reset das definicoes de wifi
int pino_reset = 4;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(pino_reset, INPUT);

  WiFiManager wifiManager;

  wifiManager.setConfigPortalTimeout(240);

  //Cria um AP (Access Point) com: ("nome da rede", "senha da rede")
  if (!wifiManager.autoConnect("ESP_ARDUINOECIA", "arduinoecia")) {
    Serial.println(F("Falha na conexao. Resetar e tentar novamente..."));
    delay(3000);
    ESP.restart();
    delay(5000);
  }

  //Mensagem caso conexao Ok
  Serial.println(F("Conectado na rede Wifi."));
  Serial.print(F("Endereco IP: "));
  Serial.println(WiFi.localIP());
}

void loop() {
  //Verifica se o botao foi pressionado
  int valor = digitalRead(pino_reset);
  if (valor == 1) {
    //Apaga os dados da rede wifi gravados na memoria e reinicia o ESP
    WiFiManager wifiManager;
    wifiManager.resetSettings();
    Serial.println("Configuracoes zeradas!");
    ESP.restart();
  }
}