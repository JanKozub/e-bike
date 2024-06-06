#include <mcp_can.h>
#include "vesc_can_bus_arduino.h"

CAN can;             // get torque sensor data, throttle for now

bool print_realtime_data = true;
long last_print_data;


void setup()
{
    while(!Serial);
    Serial.begin(115200); // Wired comms from USB port
    pinMode(CAN0_INT, INPUT);
    can.initialize();

    Serial.begin(115200);

Serial.print("SCK:"); Serial.println(SCK);
Serial.print("MISO:"); Serial.println(MISO);
Serial.print("MOSI:"); Serial.println(MOSI);
Serial.print("SS:"); Serial.println(SS);
Serial.print("SDA:"); Serial.println(SDA);
Serial.print("SCL:"); Serial.println(SCL);
}

void loop()
{
can.spin();

    can.vesc_set_current(2); //2 amps of current

    if (print_realtime_data == true)
    {
        if (millis() - last_print_data > 200)
        {
            Serial.print(can.erpm); 
            Serial.print(" ");
            Serial.print(can.inpVoltage);
            Serial.print(" ");
            Serial.print(can.dutyCycleNow);
            Serial.print(" ");
            Serial.print(can.avgInputCurrent);
            Serial.print(" ");
            Serial.print(can.avgMotorCurrent);
            Serial.print(" ");
            Serial.print(can.tempFET);
            Serial.print(" ");
            Serial.print(can.tempMotor);
            Serial.println("");

            last_print_data = millis();
        }
    }
}