void updateCanValues() {
  can.readCan();

  if (esp_timer_get_time() - lastCanRead > CAN_REFRESH_RATE * 1000) {
    inputVoltage = can.inpVoltage;
    inputCurrent = can.avgMotorCurrent;
    dutyCycle = getDutyCycle();
    vescTemp = can.tempFET;
    motorTemp = can.tempMotor;
    motorCurrent = can.avgMotorCurrent;

    lastCanRead = esp_timer_get_time();
  }
}

float getBatteryProc() {
  batteryProcentage = (inputVoltage - MIN_BATTERY_VOLTAGE) / batteryVoltageRange;
  return batteryProcentage > 1 ? 100 : batteryProcentage * 100;
}

float getDutyCycle() {
  return can.dutyCycleNow < 0 ? 0 : can.dutyCycleNow * 100;
}