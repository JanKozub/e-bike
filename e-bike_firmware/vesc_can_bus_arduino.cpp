#include <mcp_can.h>
#include <SPI.h>
#include "vesc_can_bus_arduino.h"

#define CAN_CS 12

MCP_CAN CAN0(CAN_CS);

void CAN::initialize() {
  CAN0.begin(MCP_ANY, CAN_250KBPS, MCP_8MHZ);
  CAN0.setMode(MCP_NORMAL);
}

void CAN::readCan() {
  get_frame();

  switch (rxId) {
    case 0x8000090A:
      dutyCycleNow = process_data_frame_vesc('D', rxBuf[6], rxBuf[7]);
      avgMotorCurrent = process_data_frame_vesc('C', rxBuf[4], rxBuf[5]);
      unsigned char erpmvals[4];
      erpmvals[0] = rxBuf[3];
      erpmvals[1] = rxBuf[2];
      erpmvals[2] = rxBuf[1];
      erpmvals[3] = rxBuf[0];
      erpm = *(long *)erpmvals;
      break;
    case 0x80000F0A:
      unsigned char WHvals[4];
      WHvals[0] = rxBuf[3];
      WHvals[1] = rxBuf[2];
      WHvals[2] = rxBuf[1];
      WHvals[3] = rxBuf[0];
      WattHours = *(long *)WHvals;
      break;
    case 0x8000100A:
      tempFET = process_data_frame_vesc('F', rxBuf[0], rxBuf[1]);
      tempMotor = process_data_frame_vesc('T', rxBuf[2], rxBuf[3]);
      avgInputCurrent = process_data_frame_vesc('I', rxBuf[4], rxBuf[5]);
      break;
    case 0x80001B0A:
      char receivedByte[4], *p;
      sprintf(receivedByte, "%02X%02X", rxBuf[4], rxBuf[5]);
      inpVoltage = hex2int(receivedByte) * 0.1;
      break;
  }
  //print_raw_can_data();
}

void CAN::print_raw_can_data() {
  int len = 8;
  sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);
  Serial.print(msgString);
  for (byte i = 0; i < len; i++) {
    sprintf(msgString, " 0x%.2X", rxBuf[i]);
    Serial.print(msgString);
  }
  Serial.println();
}

float CAN::process_data_frame_vesc(char datatype, unsigned char byte1, unsigned char byte2) {
  char receivedByte[4], *p;
  sprintf(receivedByte, "%02X%02X", byte1, byte2);
  float output = hex2int(receivedByte);

  switch (datatype) {
    case 'D': output *= 0.001; break;  //dutyCycleNow
    case 'C': output *= 0.1; break;    //avgMotorCurrent
    case 'F': output *= 0.1; break;    //tempFET
    case 'T': output *= 0.1; break;    //tempMotor
    case 'I': output *= 0.1; break;    //avgInputCurrent
    case 'V': output *= 0.1; break;    //inpVoltage
  }
  return output;
}

int CAN::hex2int(char buf[]) {
  return (short)strtol(buf, NULL, 16);
}

void CAN::get_frame() {
  CAN0.readMsgBuf(&rxId, &len, rxBuf);
}
