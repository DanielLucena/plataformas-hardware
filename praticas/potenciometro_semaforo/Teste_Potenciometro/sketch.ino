int valorLido = 0;
int y = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);

  pinMode(10, OUTPUT);
  pinMode(9, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  valorLido = analogRead(A5);
  y = map(valorLido, 0, 1023, 0, 100);
  // Serial.println(y);
  // delay(200);
  y>30 ? digitalWrite(6, HIGH) : digitalWrite(6, LOW);
  y>60 ? digitalWrite(7, HIGH) : digitalWrite(7, LOW);
  y>90 ? digitalWrite(8, HIGH) : digitalWrite(8, LOW);

  if(digitalRead(9) == HIGH){
    digitalWrite(10, HIGH);
    delay(500);
    digitalWrite(10, LOW);
  }
  
}
