#include <FollowerApp.h>
#include <RobotDefinitions.h>
#include <DistanceSensor.h>
#include <Chassis.h>
#include <MorseGenerator.h>
#include <SoundDetection.h>

void FollowerAppTask_Run(void)
{

    int minID = SENS_US_CENTER_ID;
    int minDist = sensorDistance[SENS_US_CENTER_ID].distance;

    if (minDist > sensorDistance[SENS_US_LEFT_ID].distance)
    {
        minID = SENS_US_LEFT_ID;
        minDist = sensorDistance[SENS_US_LEFT_ID].distance;
    }

    if (minDist > sensorDistance[SENS_US_RIGHT_ID].distance)
    {
        minID = SENS_US_RIGHT_ID;
        minDist = sensorDistance[SENS_US_RIGHT_ID].distance;
    }

    // Bumper diag
    if (!chassisBusy)
    {
        if (minID == SENS_US_LEFT_ID)
        { // Angle Under
            ChassisNavSpeedInit();
            ChassisTurnLeft(NAV_TURN_ANGLE);
        }
        else if (minID == SENS_US_RIGHT_ID)
        { // Angle Over
            ChassisNavSpeedInit();
            ChassisTurnRight(NAV_TURN_ANGLE);
        }
        else if (minDist > DISTANCE_MAX)
        { // Distance Over
            ChassisStepFWD(NAV_WALK_STEP);
            ChassisNavSpeedUp();
        }
        else if (minDist < DISTANCE_MIN)
        { // Distance under
            ChassisNavSpeedInit();
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
