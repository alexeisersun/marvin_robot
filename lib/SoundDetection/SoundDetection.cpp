#include <SoundDetection.h>
#include <Arduino.h>

char Get_SoundDetRaw(void)
{
    return sensorSoundDet.dataRaw;
}

void SoundDetTask_Init(void)
{
    sensorSoundDet.pin = SENS_SOUND_DET_PIN;
    pinMode(SENS_SOUND_DET_PIN, INPUT);
}

void SoundDetTask_Run(void)
{
    // Acquisition
    sensorSoundDet.dataRaw = digitalRead(SENS_SOUND_DET_PIN);

    // Evaluation
}