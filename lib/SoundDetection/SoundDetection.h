#ifndef SOUND_DETECTION_H_
#define SOUND_DETECTION_H_

#include <Arduino.h>
#include <RobotDefinitions.h>
#include <Sensor.h>

extern struct binary_sensor_str sensorSoundDet;

char Get_SoundDetRaw(void);
void SoundDetTask_Init(void);
void SoundDetTask_Run(void);
#endif