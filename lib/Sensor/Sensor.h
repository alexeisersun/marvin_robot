#ifndef SENSOR_H_
#define SENSOR_H_

#include <Arduino.h>
#include <RobotDefinitions.h>

struct binary_sensor_str
{
    uint8_t pin;
    uint8_t dataRaw;

    int8_t abcCnt;
    int8_t abcMax;
    int8_t abcMin;

    uint8_t dataFilt;
};

#endif