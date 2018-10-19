#ifndef LINE_SENSOR_H_
#define LINE_SENSOR_H_

#include <RobotDefinitions.h>
#include <Sensor.h>

struct digital_sensor_str
{
    uint16_t pin;
    uint16_t dataRaw;
};

extern struct digital_sensor_str sensorLine[LINE_SENSOR_NR_OF];

void AnalogSensorEval(struct digital_sensor_str*);
void LineSensorTask_Init(void);
void LineSensorTask_Run(void);
#endif