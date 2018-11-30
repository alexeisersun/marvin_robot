#ifndef BUMPER_SENSOR_H_
#define BUMPER_SENSOR_H_

#include <Arduino.h>
#include <RobotDefinitions.h>
#include <Sensor.h>

extern struct binary_sensor_str sensorBumper[BUMPER_NR_OF];

char Get_BumperRaw(uint8_t sensorId);
char Get_BumperAbcCnt(uint8_t sensorId);
char Get_BumperFilt(uint8_t sensorId);
void BumperTask_Init(void);
void BinarySensorEval(struct binary_sensor_str *sensor);
void BumperTask_Run(void);

#endif