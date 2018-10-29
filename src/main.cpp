#include <MsTimer2.h>
#include <RobotDefinitions.h>
#include <Sensor.h>
#include <LineSensor.h>
#include <DistanceSensor.h>
#include <SoundDetection.h>
#include <BumperSensor.h>
#include <MorseGenerator.h>
#include <SoundDetection.h>
#include <Chassis.h>

char APP_STATE = APP_STATE_MANUAL;
char followerEnable = 0;

struct TaskContext_str
{
    int REQURENCY;
    int OFFSET;
    int ReqCnt;
    void (*execute)(void);
};

struct TaskContext_str TASK_List[NR_OF_TASKS] = {
    {CHASSIS_REQ, 3, 0, ChassisTask_Run},
    {1, 0, 0, SoundDetTask_Run},
    {10, 1, 0, BumperTask_Run},
    {100, 2, 0, LineSensorTask_Run}};

int TestCnt = 0;

void TaskScheduler(void)
{

    for (int taskNr = 0; taskNr < NR_OF_TASKS; taskNr++)
    {
        if (--TASK_List[taskNr].ReqCnt <= 0)
        {
            TASK_List[taskNr].execute();
            TASK_List[taskNr].ReqCnt = TASK_List[taskNr].REQURENCY;
        }
    }
}

void SysTick(void)
{
    TaskScheduler();
}

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

void setup()
{
    CTRL_SERIAL.begin(9600);
    MON_SERIAL.begin(9600);
    pulseIn();
    pinMode(13, OUTPUT);

    SoundDetTask_Init();
    BumperTask_Init();
    LineSensorTask_Init();
    DistanceSensorTask_Init();

    MsTimer2::set(SYS_TICK_TIME, SysTick);
    MsTimer2::start();
}

void loop()
{
    DistanceSensorTask_Run();

    if (APP_STATE == APP_STATE_AUTO_FOLLOW)
    {
        FollowerAppTask_Run();
    }
    else if (APP_STATE == APP_STATE_AUTO_TUNEL)
    {
        TunelAppTask_Run();
    }

    if (CTRL_SERIAL.available())
    {
        char incommingByte = CTRL_SERIAL.read();
        switch (incommingByte)
        {

        case '0':
            CTRL_SERIAL.println("Am receptiont Comanda 0");
            PlayMorseString("SOS");
            break;

        case '1':
            CTRL_SERIAL.println("Am receptiont Comanda 1 - Manual Control");
            APP_STATE = APP_STATE_MANUAL;
            break;

        case '2':
            CTRL_SERIAL.println("Am receptiont Comanda 2 - Follower");
            APP_STATE = APP_STATE_AUTO_FOLLOW;
            break;

        case '3':
            CTRL_SERIAL.println("Am receptiont Comanda 3 - Tunel");
            APP_STATE = APP_STATE_AUTO_TUNEL;
            break;

        case 'P':
            CTRL_SERIAL.println("Am receptiont Comanda P - PROGRAM");
            ChassisNavProgram("FFFLRFFFLBBBBBB");
            break;

        case 'Y':
            CTRL_SERIAL.println("Am receptiont Comanda y - Play Log");
            ChassisNavProgram(log_program);
            break;

        case 'X':
            CTRL_SERIAL.println("Am receptiont Comanda x - clean");
            log_program[0] = 0;
            break;

        case 'C':
            CTRL_SERIAL.println("Am receptiont Comanda c - Copy Log");
            strcpy(nav_program, log_program);
            break;

        case 'F':
            CTRL_SERIAL.println("Am receptiont Comanda FWD");
            ChassisStepFWD(NAV_WALK_STEP);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        case 'B':
            CTRL_SERIAL.println("Am receptiont Comanda BWD");
            ChassisStepBWD(NAV_WALK_STEP);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }

            break;

        case 'L':
            CTRL_SERIAL.println("Am receptiont Comanda LEFT");
            ChassisTurnLeft(NAV_TURN_ANGLE);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        case 'R':
            CTRL_SERIAL.println("Am receptiont Comanda RIGHT");
            ChassisTurnRight(NAV_TURN_ANGLE);
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        case 'S':
            CTRL_SERIAL.println("Am receptiont Comanda STOP");
            ChassisStop();
            if (log_program_it < NAV_PROGRAM_SIZE)
            {
                log_program[log_program_it++] = incommingByte;
                log_program[log_program_it] = 0;
            }
            break;

        default:
            CTRL_SERIAL.println("Comanda necunoscuta ");
        }
    }
}