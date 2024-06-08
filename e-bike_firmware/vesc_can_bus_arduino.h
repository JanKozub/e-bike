#include <Arduino.h>

class CAN {

public:
  long unsigned int rxId;
  unsigned char len = 0;
  unsigned char rxBuf[8];
  char msgString[128];  // Array to store serial string

  float inpVoltage, dutyCycleNow, avgInputCurrent, avgMotorCurrent, tempFET, tempMotor;
  long erpm, WattHours;

  void initialize();
  void readCan();
  void get_frame();             // populates rxId and rxBuf with latest can frame
  void can_send(byte data[8]);  //transmits the send commands to the sensor
  void print_raw_can_data();    //output raw can data to terminal (debug)

  float process_data_frame_vesc(char datatype, unsigned char byte1, unsigned char byte2);
  int hex2int(char buf[]);
};
