#ifndef DISTANCE_SENSOR_H_
#define DISTANCE_SENSOR_H_

#include <Arduino.h>
#include <RobotDefinitions.h>

struct distance_sensor_str
{
    uint16_t trigPin;
    uint16_t echoPin;
    uint32_t duration;
    uint32_t distance;
    uint16_t distanceBuffer[DIST_BUFF_SIZE];
    uint16_t buffPos;
};

extern struct distance_sensor_str sensorDistance[DISTANCE_SENSOR_NR_OF];

#endif