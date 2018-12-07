#include <ConcurrentBumperApp.h>

struct PulseData bumperBuffer[CONPULSE_NUM_SENSORS];
uint8_t bumperSensorMask = 0b00001111;

void ConcurrentBumperApp_Init(void)
{
    setupDistanceSensors(bumperSensorMask);
}

void ConcurrentBumperApp_Run(void)
{
    getDistances(&bumperBuffer, bumperSensorMask);
  
    for(int i = 0; i < CONPULSE_NUM_SENSORS; i++)
    {
        Serial.print(bumperBuffer[i].index);
        Serial.print(' ');
        Serial.println(bumperBuffer[i].distance / 58);
    }
  
    Serial.println();
}
