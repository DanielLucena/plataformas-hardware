#define tStart 27
#define touchJogadorUm 33
#define touchJogadorDois 32

#define ledStart 12
#define ledJogadorUm 13
#define ledJogadorDois 14

int scoreJogadorUm = 0, scoreJogadorDois = 0;
bool jogadorUmQueimou = false, jogadorDoisQueimou = false;

void setup() {
  Serial.begin(115200);
  delay(1000);
  //Serial.println("ESP Touch Test");
  pinMode(ledStart, OUTPUT);
  pinMode(ledJogadorUm, OUTPUT);
  pinMode(ledJogadorDois, OUTPUT);
  Serial.println("Toque no primeiro cabo para começar o jogo");
}

void loop() {
  if (touchRead(tStart) <= 20) {
    rodada();
  }
  delay(1000);
}


void ledPulse(int LED_PIN, int tempo, int piscadas) {
  digitalWrite(LED_PIN, LOW);
  int cycle = tempo / piscadas / 2;
  for (int i = 0; i < piscadas; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(cycle);
    digitalWrite(LED_PIN, LOW);
    delay(cycle);
  }
  digitalWrite(LED_PIN, HIGH);
}

void rodada() {
  jogadorUmQueimou = false;
  jogadorDoisQueimou = false;
  digitalWrite(ledJogadorUm, LOW);
  digitalWrite(ledJogadorDois, LOW);
  int pulseTime = 3000;
  bool jogadorUmGanhou;
  int tempoFinal = 0;
  ledPulse(ledStart, pulseTime, 3);
  int tempoInicial = millis();

  //para evitar queimar a largada
  if (touchRead(touchJogadorUm) <= 20) {
    jogadorUmQueimou = true;
    Serial.println("Jogador Um queimou a largada");
  }
  if (touchRead(touchJogadorDois) <= 20) {
    jogadorDoisQueimou = true;
    Serial.println("Jogador Dois queimou a largada");
  }

  while (true) {
    if ((touchRead(touchJogadorUm) <= 20) and !jogadorUmQueimou) {
      digitalWrite(ledJogadorUm, HIGH);
      jogadorUmGanhou = true;
      scoreJogadorUm++;
      break;
    }
    if ((touchRead(touchJogadorDois) <= 20) and !jogadorDoisQueimou) {
      digitalWrite(ledJogadorDois, HIGH);
      jogadorUmGanhou = false;
      scoreJogadorDois++;
      break;
    }
    if (jogadorUmQueimou and jogadorDoisQueimou) {
      break;
    }
  }
  tempoFinal = millis();
  digitalWrite(ledStart, LOW);
  if (jogadorUmQueimou and jogadorDoisQueimou) {
    Serial.println("Ambos os jogadores queimaram a largada");
  }
  else if (jogadorUmGanhou ) {
    Serial.print("Jogador Um Ganhou! ");
    Serial.println("em " + String(tempoFinal - tempoInicial) + " milisegundos");
  }
  else{
    Serial.print("Jogador Dois Ganhou! ");
    Serial.println("em " + String(tempoFinal - tempoInicial) + " milisegundos");
  }

  Serial.println("Placar: Jogador Um " + String(scoreJogadorUm) + " X " + String(scoreJogadorDois) + " Jogador Dois");
  delay(3000);
  Serial.println("Toque no primeiro cabo para jogar novamente");

}
