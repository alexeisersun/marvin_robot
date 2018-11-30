#include <Arduino.h>
#include <LineSensor.h>

digital_sensor_str sensorLine[LINE_SENSOR_NR_OF];

void AnalogSensorEval(struct digital_sensor_str *sensor)
{
    uint16_t dataRawLcl;
    dataRawLcl = analogRead(sensor->pin);
    sensor->dataRaw = dataRawLcl;
}

void LineSensorTask_Init(void)
{
    sensorLine[SENS_BUMPER_LEFT_ID].pin = SENS_LINE_FAR_LEFT;
    sensorLine[SENS_LINE_LEFT_ID].pin = SENS_LINE_LEFT;
    sensorLine[SENS_LINE_CENTER_ID].pin = SENS_LINE_CENTER;
    sensorLine[SENS_LINE_RIGHT_ID].pin = SENS_LINE_RIGHT;
    sensorLine[SENS_LINE_FAR_RIGHT_ID].pin = SENS_LINE_FAR_RIGHT;
}

void LineSensorTask_Run(void)
{
    for (int i = 0; i < LINE_SENSOR_NR_OF; i++)
    {
        AnalogSensorEval(&sensorLine[i]);
    }
}