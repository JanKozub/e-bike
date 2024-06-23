/*
  e-bike computer firmware by Jan Kozub

  pinout:
  GPIO01 -> button

  GPIO05 -> INT(CAN_BUS)
  GPIO07 -> SCK(CAN_BUS)
  GPIO09 -> MISO(CAN_BUS)
  GPIO11 -> MOSI(CAN_BUS)
  GPIO12 -> CS(CAN_BUS)

  GPIO33 -> SDA(I2C)
  GPIO35 -> SCL(I2C)
*/

#include <U8g2lib.h>
#include <Wire.h>
#include <mcp_can.h>
#include "vesc_can_bus_arduino.h"

#define MENU_BUTTON_PIN 1
#define PAGES_AMOUNT 3

#define SDA_PIN 33
#define SCL_PIN 35

#define CAN0_INT 5
#define CAN_REFRESH_RATE 25  // time in ms

#define MIN_BATTERY_VOLTAGE 25.6
#define MAX_BATTERY_VOLTAGE 33.6

enum AlignType {
  CENTER,
  RIGHT
};

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE, SCL_PIN, SDA_PIN);
CAN can;

int currentPage = 0;
bool menuButtonState = true;
unsigned long lastCanRead = 0;
float inputVoltage, dutyCycle, inputCurrent, motorCurrent, vescTemp, motorTemp;
float batteryVoltageRange = MAX_BATTERY_VOLTAGE - MIN_BATTERY_VOLTAGE;

void setup(void) {
  pinMode(MENU_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CAN0_INT, INPUT);

  u8g2.begin();
  can.initialize();
  Serial.begin(115200);
}

void loop(void) {
  updateCanValues();
  handleButtonPress();
  printScreen();
}