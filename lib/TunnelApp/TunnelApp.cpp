#include <TunnelApp.h>
#include <RobotDefinitions.h>
#include <DistanceSensor.h>
#include <Chassis.h>
#include <MorseGenerator.h>
#include <SoundDetection.h>

void TunelAppTask_Run(void)
{

    int maxID = SENS_US_CENTER_ID;
    int maxDist = sensorDistance[SENS_US_CENTER_ID].distance;

    if (maxDist < sensorDistance[SENS_US_LEFT_ID].distance)
    {
        maxID = SENS_US_LEFT_ID;
        maxDist = sensorDistance[SENS_US_LEFT_ID].distance;
    }

    if (maxDist < sensorDistance[SENS_US_RIGHT_ID].distance)
    {
        maxID = SENS_US_RIGHT_ID;
        maxDist = sensorDistance[SENS_US_RIGHT_ID].distance;
    }

    // Bumper diag
    if (!chassisBusy)
    {
        // if (Get_BumperFilt(SENS_BUMPER_LEFT_ID) ||
        // Get_BumperFilt(SENS_BUMPER_RIGHT_ID)) {
        // // bumper reaction
        // ChassisStepFWD(NAV_WALK_STEP);
        // } else
        if (maxID == SENS_US_LEFT_ID)
        { // Angle Under
            ChassisNavSpeedInit();
            ChassisTurnLeft(NAV_TURN_ANGLE);
        }
        else if (maxID == SENS_US_RIGHT_ID)
        { // Angle Over
            ChassisNavSpeedInit();
            ChassisTurnRight(NAV_TURN_ANGLE);
        }
        else if (maxDist > DISTANCE_MAX)
        { // Distance Over
            ChassisStepFWD(NAV_WALK_STEP);
            ChassisNavSpeedUp();
        }
        else if (maxDist < DISTANCE_MIN)
        { // Distance under
            ChassisStepBWD(NAV_WALK_STEP);
        }
        else
        {
            ChassisStop();

            if (Get_SoundDetRaw())
            {
                PlayMorseString("SOS");
                ChassisNavProgram("LLFF");
            }
        }
    }
}