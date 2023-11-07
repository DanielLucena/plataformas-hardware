//Inclui as bibliotecas do SPI e do RC522
#include <SPI.h>
#include <MFRC522.h>
 
//Definição dos pinos de conexão do projeto
#define PINO_SS 9
#define PINO_RST 8
 
int led_red = 7;
int led_green = 6;
int buz = 5;
int incomingByte = 0;
int cont = 0;
bool isLiberado = false;
 
MFRC522 mfrc522(PINO_SS, PINO_RST);
 
void setup() {
  // put your setup code here, to run once:
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(buz, OUTPUT);
  Serial.begin(9600);
  SPI.begin();// Inicializa a comunicação SPI
  mfrc522.PCD_Init(); // Inicializa o módulo MFRC522
  Serial.println("Sistema Inicializado: Aproxime o token");
 
}
 
void loop() {
    digitalWrite(led_red, HIGH);
 
  // put your main code here, to run repeatedly:
  if (!mfrc522.PICC_IsNewCardPresent()) return;// Aguarda a aproximação do token
 if (!mfrc522.PICC_ReadCardSerial()) return; // Seleciona qual token vai ser utilizado
 Serial.print("UID da tag:"); // Mostra UID do token na serial
 String conteudo= ""; //Cria uma variável vazia, do tipo string
 byte letra;
 for (byte i = 0; i < mfrc522.uid.size; i++) {
 if(mfrc522.uid.uidByte[i] < 0x10){
 Serial.print(" 0");
 }
 else{
 Serial.print(" ");
 }
 Serial.print(mfrc522.uid.uidByte[i], HEX);// Printa a mensagem convertida em hexadecimal
 if(mfrc522.uid.uidByte[i] < 0x10){
 conteudo.concat(String(" 0"));
 }
 else{
 conteudo.concat(String(" "));
 }
 conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
 }
 Serial.println();
 conteudo.toUpperCase();//Coloca todas as letras da string em maiúscula
 if (conteudo.substring(1) == "8C 65 EF 66") {
 Serial.println("Acesso Permitido!");
 isLiberado = true;
 }
 delay(1000);
 
 
 
//  Serial.println("Insira a senha de acesso:");
//  while (Serial.available() == 0) {}     //wait for data available
//  String teststr = Serial.readString();  //read until timeout
//  teststr.trim();                        // remove any \r \n whitespace at the end of the String
  if (isLiberado) {
    cont = 0;
    digitalWrite(led_red, LOW);
    digitalWrite(led_green, HIGH);
//    digitalWrite(buz, HIGH);
//    delay(500);
//    digitalWrite(buz, LOW);
    //Serial.println("Acesso liberado.");
    delay(3000);
    digitalWrite(led_green, LOW);
    isLiberado = false;
  } else {
    cont++;
    Serial.println("Cartão inválido.");
    for(int i = 0; i < 3; i++) {
      digitalWrite(led_red, HIGH);
      delay(200);
      digitalWrite(led_red, LOW);
      delay(200);
    }
    if(cont >= 2) {
      Serial.println("SISTEMA BLOQUEADO.");
      digitalWrite(buz, HIGH);
      for(int i = 0; i < 5; i++) {
        digitalWrite(led_red, HIGH);
        delay(200);
        digitalWrite(led_red, LOW);
        delay(200);
      }
      digitalWrite(buz, LOW);
      cont = 0;
    }
  }
}
