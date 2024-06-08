#define THROTTLE_PIN A2
#define OUTPUT_PIN 3

void setup() {
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(OUTPUT_PIN, OUTPUT);
}

void loop() {
  analogWrite(OUTPUT_PIN, (analogRead(THROTTLE_PIN) >> 2) - 10);
}
