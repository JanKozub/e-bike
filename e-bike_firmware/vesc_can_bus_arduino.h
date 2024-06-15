#include <Arduino.h>

class CAN {

public:
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];
  char msgString[128];

  float inpVoltage, dutyCycleNow, avgInputCurrent, avgMotorCurrent, tempFET, tempMotor;
  long erpm, WattHours;

  void initialize();
  void readCan();
  void get_frame();
  void print_raw_can_data();

  float process_data_frame_vesc(char datatype, unsigned char byte1, unsigned char byte2);
  int hex2int(char buf[]);
};
