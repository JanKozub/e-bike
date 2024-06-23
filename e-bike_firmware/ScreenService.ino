#define ALIGN_LEFT 0
#define ALIGN_CENTER(t) ((u8g2.getDisplayWidth() - (u8g2.getUTF8Width(t))) / 2)
#define ALIGN_RIGHT(t) (u8g2.getDisplayWidth() - u8g2.getUTF8Width(t))

void printScreen() {
  u8g2.clearBuffer();
  switch (currentPage) {
    case 0:
      drawMainPage();
      break;
    case 1:
      drawDevValues1();
      break;
    case 2:
      drawDevValues2();
      break;
    default:
      drawMainPage();
  }
  u8g2.sendBuffer();
}

void drawMainPage() {
  u8g2.drawBox(0, 64 - map(dutyCycle, 0, 95, 0, 64), 20, 64);
  u8g2.drawBox(108, 64 - map(inputCurrent, 0, 70, 0, 64), 20, 64);

  u8g2.setFont(u8g2_font_10x20_tr);
  printText(22, getBatteryProc(), '%', AlignType::CENTER);
  printText(42, inputCurrent, 'A', AlignType::CENTER);
  printText(62, inputVoltage, 'V', AlignType::CENTER);
}

void drawDevValues1() {
  u8g2.drawStr(ALIGN_LEFT, 22, "Inp. V:");
  printText(22, inputVoltage, 'V', AlignType::RIGHT);
  u8g2.drawStr(ALIGN_LEFT, 42, "Inp. A:");
  printText(42, inputCurrent, 'A', AlignType::RIGHT);
  u8g2.drawStr(ALIGN_LEFT, 62, "Duty C.:");
  printText(62, dutyCycle, '%', AlignType::RIGHT);
}

void drawDevValues2() {
  u8g2.drawStr(ALIGN_LEFT, 22, "Motor A:");
  printText(22, motorCurrent, 'A', AlignType::RIGHT);
  u8g2.drawStr(ALIGN_LEFT, 62, "Motor T:");
  printText(62, motorTemp, 'C', AlignType::RIGHT);
  u8g2.drawStr(ALIGN_LEFT, 42, "VESC T:");
  printText(42, vescTemp, 'C', AlignType::RIGHT);
}

void printText(int h, double data, char endChar, AlignType align) {
  char placeholder[data > 99.99 ? 7 : (data > 9.99 ? 6 : 5)];

  if (data > 99.99) {
    char temp[7] = { ' ', ' ', ' ', ' ', ' ', endChar, '\0' };
    strcpy(placeholder, temp);
  } else if (data > 9.99) {
    char temp[6] = { ' ', ' ', ' ', ' ', endChar, '\0' };
    strcpy(placeholder, temp);
  } else {
    char temp[5] = { ' ', ' ', ' ', endChar, '\0' };
    strcpy(placeholder, temp);
  }

  if (align == AlignType::RIGHT) {
    u8g2.setCursor(ALIGN_RIGHT(placeholder), h);
    u8g2.print(data, 1);
    u8g2.drawStr(ALIGN_RIGHT(placeholder), h, placeholder);
  } else {
    u8g2.setCursor(ALIGN_CENTER(placeholder), h);
    u8g2.print(data, 1);
    u8g2.drawStr(ALIGN_CENTER(placeholder), h, placeholder);
  }
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