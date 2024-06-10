#include <U8g2lib.h>
#include <Wire.h>

#define THROTTLE_PIN 3
#define MENU_BUTTON_PIN 5

void setup() {
  Serial.begin(115200);
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(MENU_BUTTON_PIN, INPUT_PULLUP);

}

void loop() {
  Serial.println(digitalRead(5));

}
