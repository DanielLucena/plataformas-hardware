#include <FS.h>
#include "SPIFFS.h"

String str;
String s;

int botaoPin = 21; 
int ledPin = 22;    

bool ledState = LOW;  // Variável para armazenar o estado do LED

void writeFile(String state, String path) { //escreve conteúdo em um arquivo
  File rFile = SPIFFS.open(path, "a");//a para anexar
  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }
  else {
    Serial.print("tamanho:");
    Serial.println(rFile.size());
    rFile.println(state);
    Serial.print("Gravou: ");
    Serial.println(state);
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


void setup() {

  pinMode(botaoPin, INPUT);
  pinMode(ledPin, OUTPUT);
 
  Serial.begin(9600);
  Serial.println("inicio");
  delay(200);
  //formatFile();
  Serial.println("abrir arquivo");
  openFS(); 

  Serial.println("ler arquivo");
  String ledStateString = readFile("/logsAula.txt");
  //Serial.print("ledStateString ");
  //Serial.println(ledStateString);

  ledState = ledStateString.toInt();
  digitalWrite(ledPin, ledState);

  //Serial.print("ledState ");
  //Serial.println(ledState);
  
  Serial.print("ledState do arquivo: ");
  Serial.println(ledState);
  delay(2000); 
  digitalWrite(ledPin, ledState); 
  delay(1000);
}

void loop() {

    while(digitalRead(botaoPin) == 0 ){
      ledState = !ledState;
      Serial.print("ledState após botão: ");
      Serial.println(ledState);
      digitalWrite(ledPin, ledState);  
      delay(1000);
      writeFile(String(ledState) , "/logsAula.txt");
      //Serial.println("ler arquivo");
      //String ledStateString = readFile("/logsAula.txt");
    }
}
