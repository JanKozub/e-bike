#define ALIGN_LEFT 0
#define ALIGN_CENTER(t) ((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (u8g2.getDisplayWidth() - u8g2.getUTF8Width(t))

void drawMainPage() {
  u8g2.clearBuffer();
  u8g2.drawBox(0, 64 - map(dutyCycle, 0, 95, 0, 64), 20, 64);
  u8g2.drawBox(108, 64 - map(40, 0, 80, 0, 64), 20, 64);

  u8g2.setFont(u8g2_font_10x20_tr);
  printText(22, (inputVoltage / MAX_BATTERY_VOLTAGE * 100), '%');
  printText(42, inputCurrent, 'A');
  printText(62, inputVoltage, 'V');

  u8g2.sendBuffer();
}

void drawDevValues1() {
  u8g2.clearBuffer();

  printText(22, inputVoltage, 'V');
  printText(42, inputCurrent, 'A');
  printText(62, dutyCycle, '%');

  u8g2.sendBuffer();
}

void drawDevValues2() {
  u8g2.clearBuffer();

  printText(22, motorCurrent, 'A');
  printText(42, vescTemp, 'C');
  printText(62, motorTemp, 'C');

  u8g2.sendBuffer();
}

void printText(int y, double data, char endChar) {
  char placeholder[data >= 10 ? 6 : 5];

  if (data >= 10) {
    char temp[6] = { ' ', ' ', ' ', ' ', endChar, '\0' };
    strcpy(placeholder, temp);
  } else {
    char temp[5] = { ' ', ' ', ' ', endChar, '\0' };
    strcpy(placeholder, temp);
  }

  u8g2.setCursor(ALIGN_CENTER(placeholder), y);
  u8g2.print(data, 1);
  u8g2.drawStr(ALIGN_CENTER(placeholder), y, placeholder);
}

void handleButtonPress() {
  if (digitalRead(MENU_BUTTON_PIN) == 0) {
    menuButtonState = false;
  } else if (!menuButtonState) {
    menuButtonState = true;
    currentPage++;
    if (currentPage == PAGES_AMOUNT) currentPage = 0;
  }
}
