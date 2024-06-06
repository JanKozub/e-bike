void setup() {
  pinMode(A2, INPUT);
  pinMode(3, OUTPUT);
}

void loop() {
  analogWrite(3, map(analogRead(A2), 0, 1023, 0, 255));
}
