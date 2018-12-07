#ifndef CONCURRENT_BUMPER_APP_H_
#define CONCURRENT_BUMPER_APP_H_

#include <ConcurrentPulse.h>
#include <stdint.h>

extern struct PulseData bumperBuffer[CONPULSE_NUM_SENSORS];
extern uint8_t bumperSensorMask;

void ConcurrentBumperApp_Init(void);

// Aquire data from sensors and send them over serial connnection.
void ConcurrentBumperApp_Run(void);

#endif