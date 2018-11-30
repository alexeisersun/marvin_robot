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
#include <FollowerApp.h>
#include <TunnelApp.h>
#include <TaskScheduler.h>

char APP_STATE = APP_STATE_MANUAL;
char followerEnable = 0;

void SysTick(void)
{
    TaskScheduler();
}

void setup()
{
    CTRL_SERIAL.begin(9600);
    MON_SERIAL.begin(9600);
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