#include <DistanceSensor.h>

int distsort(const void *p, const void *q)
{
    int l = *(const int *)p;
    int r = *(const int *)q;

    if (l > r)
        return 1;
    if (l == r)
        return 0;
    return -1;
}

void DistanceSensorEval(struct distance_sensor_str *sensor)
{
    long duration;
    int distance;
    digitalWrite(sensor->trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(sensor->trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(sensor->trigPin, LOW);
    duration = pulseIn(sensor->echoPin, HIGH, MAX_DUR);

    if (duration <= 0)
        duration = MAX_DUR;
    distance = duration * 0.034 / 2;

    if (++sensor->buffPos >= DIST_BUFF_SIZE)
        sensor->buffPos = DIST_BUFF_SIZE;
    sensor->distanceBuffer[sensor->buffPos] = distance;

    sensor->duration = duration;
    sensor->distance = distance;
}

void DistanceSensorTask_Init(void)
{
    sensorDistance[SENS_US_LEFT_ID].trigPin = SENS_US_TRIG_LEFT_PIN;
    sensorDistance[SENS_US_LEFT_ID].echoPin = SENS_US_ECHO_LEFT_PIN;

    sensorDistance[SENS_US_CENTER_ID].trigPin = SENS_US_TRIG_CENTER_PIN;
    sensorDistance[SENS_US_CENTER_ID].echoPin = SENS_US_ECHO_CENTER_PIN;

    sensorDistance[SENS_US_RIGHT_ID].trigPin = SENS_US_TRIG_RIGHT_PIN;
    sensorDistance[SENS_US_RIGHT_ID].echoPin = SENS_US_ECHO_RIGHT_PIN;
    for (int i = 0; i < DISTANCE_SENSOR_NR_OF; i++)
    {
        pinMode(sensorDistance[i].trigPin, OUTPUT);
        pinMode(sensorDistance[i].echoPin, INPUT);
    }
}

void DistanceSensorTask_Run(void)
{
    for (int i = 0; i < DISTANCE_SENSOR_NR_OF; i++)
    {
        DistanceSensorEval(&sensorDistance[i]);
    }
}

