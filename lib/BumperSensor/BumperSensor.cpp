#include <BumperSensor.h>

struct binary_sensor_str sensorBumper[BUMPER_NR_OF] = {
    // pin / dataRaw / abcCnt / abcMax / abcMin / dataFilt
    {SENS_BUMPER_LEFT_PIN, 0, 0, 4, 0, 0},
    {SENS_BUMPER_RIGHT_PIN, 0, 0, 4, 0, 0}};

char Get_BumperRaw(uint8_t sensorId)
{
    return sensorBumper[sensorId].dataRaw;
}
char Get_BumperAbcCnt(uint8_t sensorId)
{
    return sensorBumper[sensorId].abcCnt;
}
char Get_BumperFilt(uint8_t sensorId)
{
    return (sensorBumper[sensorId].dataFilt == 0);
}

void BumperTask_Init(void)
{
    sensorBumper[SENS_BUMPER_LEFT_ID].pin = SENS_BUMPER_LEFT_PIN;
    sensorBumper[SENS_BUMPER_RIGHT_ID].pin = SENS_BUMPER_RIGHT_PIN;

    for (int i = 0; i < BUMPER_NR_OF; i++)
    {
        pinMode(sensorBumper[i].pin, INPUT);
    }
}

void BinarySensorEval(struct binary_sensor_str *sensor)
{
    uint8_t dataRawLcl;
    // Acquisition
    dataRawLcl = digitalRead(sensor->pin);
    sensor->dataRaw = dataRawLcl;
    // Evaluation
    if (dataRawLcl > 0)
    {
        if (++sensor->abcCnt > sensor->abcMax)
        {
            sensor->abcCnt = sensor->abcMax; // saturation
        }
    }
    else
    {
        if (--sensor->abcCnt < sensor->abcMin)
        {
            sensor->abcCnt = sensor->abcMin; // saturation
        }
    }

    if (sensor->abcCnt <= sensor->abcMin)
    {
        sensor->dataFilt = 0;
    }
    else if (sensor->abcCnt >= sensor->abcMax)
    {
        sensor->dataFilt = 1;
    }
    else
    {
        // do nothing, keep the last value
    }
}

void BumperTask_Run(void)
{
    for (int i = 0; i < BUMPER_NR_OF; i++)
    {
        BinarySensorEval(&sensorBumper[i]);
    }
}