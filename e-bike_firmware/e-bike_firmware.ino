#include <U8g2lib.h>
#include <Wire.h>

#define THROTTLE_PIN 3
#define MENU_BUTTON_PIN 5

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, 35, 33);

#define LCDWidth u8g2.getDisplayWidth()
#define ALIGN_CENTER(t) ((LCDWidth - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (LCDWidth - u8g2.getUTF8Width(t))
#define ALIGN_LEFT 0

int currentPage = 0;
bool menuButtonState = false;
unsigned long pushTime = 0;
unsigned long releaseTime = 0;
int throttleRange[] = { 0, 8192 };

double battery = 85.5;
double current = 20.1;
double voltage = 44.1;

void setup(void) {
  u8g2.begin();
  Serial.begin(115200);
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(MENU_BUTTON_PIN, INPUT_PULLUP);
}

void loop(void) {
  switch (currentPage) {
    case 0:
      drawMainPage();
      break;
    case 1:
      u8g2.clearBuffer();
      break;
    default:
      drawMainPage();
  }
}

void drawMainPage() {
  u8g2.clearBuffer();
  u8g2.drawBox(0, 64 - map(analogRead(THROTTLE_PIN), throttleRange[0], throttleRange[1], 0, 64), 20, 64);  //TODO filter noise on input
  u8g2.drawBox(108, 64 - map(20.2, 0, 80, 0, 64), 20, 64);

  u8g2.setFont(u8g2_font_10x20_tr);
  printText(22, battery, '%');
  printText(42, current, 'A');
  printText(62, voltage, 'V');

  u8g2.sendBuffer();

  if (pushTime == 0 && digitalRead(MENU_BUTTON_PIN) == 0) {
    pushTime = esp_timer_get_time();
  }

  if (pushTime > 0 && digitalRead(MENU_BUTTON_PIN) == 1) {
    if (esp_timer_get_time() - pushTime > 5000000) {
      configThrottle();
    } else {
      currentPage++;
    }

    pushTime = 0;
  }
}

void configThrottle() {
  throttleRange[0] = 4000;
  throttleRange[1] = 4000;
  while (digitalRead(MENU_BUTTON_PIN) == 1) {
    int val = analogRead(THROTTLE_PIN);
    if (val < throttleRange[0]) throttleRange[0] = val;
    if (val > throttleRange[1]) throttleRange[1] = val;
  }
}

void printText(int y, double data, char endChar) {
  if (data > 10) {
    char placeholder[6] = { ' ', ' ', ' ', ' ', endChar, '\0'};

    u8g2.setCursor(ALIGN_CENTER(placeholder), y);
    u8g2.print(data, 1);
    u8g2.drawStr(ALIGN_CENTER(placeholder), y, placeholder);
  }
}
