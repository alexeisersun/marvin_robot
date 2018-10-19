#include <Chassis.h>

void ChassisNavSpeedUp(void)
{
    if (++nav_speed > NAV_SPEED_MAX)
        nav_speed = NAV_SPEED_MAX;
}

void ChassisNavSpeedInit(void)
{
    nav_speed = NAV_SPEED;
}

void ChassisStop(void)
{ // steps in ms
    // stop motors
    motorRight.stop();
    motorLeft.stop();
}

void ChassisStepFWD(int steps)
{ // steps in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.forward();
    motorLeft.forward();

    // running for
    chassisNavTime = steps;
}

void ChassisStepBWD(int steps)
{ // steps in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.backward();
    motorLeft.backward();

    // running for
    chassisNavTime = steps;
}

void ChassisTurnLeft(int angle)
{ // angle in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.forward();
    motorLeft.backward();

    // running for
    chassisNavTime = angle;
}

void ChassisTurnRight(int angle)
{ // angle in ms

    // set the speed
    motorRight.setSpeed(NAV_SPEED + NAV_SPEED_COR_R);
    motorLeft.setSpeed(NAV_SPEED + NAV_SPEED_COR_L);

    // stert mottors
    motorRight.backward();
    motorLeft.forward();

    // running for
    chassisNavTime = angle;
}

void ChassisNavProgram(char program[])
{
    int i = 0;

    while (program[i] && i < NAV_PROGRAM_SIZE)
    {
        nav_program[i] = program[i];
        i++;
    }
    chassisProgram = 1;
    progIt = 0;

    MON_SERIAL.println(nav_program);
}

void ChassisTask_Run(void)
{
    if (chassisProgram && !chassisBusy)
    {

        if (nav_program[progIt])
        {
            switch (nav_program[progIt])
            {

            case 'F':
                CTRL_SERIAL.println("Am receptiont Comanda FWD");
                ChassisStepFWD(PROG_WALK_STEP);
                break;

            case 'B':
                CTRL_SERIAL.println("Am receptiont Comanda BWD");
                ChassisStepBWD(PROG_WALK_STEP);
                break;

            case 'L':
                CTRL_SERIAL.println("Am receptiont Comanda LEFT");
                ChassisTurnLeft(PROG_TURN_ANGLE);
                break;

            case 'R':
                CTRL_SERIAL.println("Am receptiont Comanda RIGHT");
                ChassisTurnRight(PROG_TURN_ANGLE);
                break;

            case 'S':
                CTRL_SERIAL.println("Am receptiont Comanda STOP");
                ChassisStop();
                break;
            }
            progIt++;
        }
        else
        {
            chassisProgram = 0;
            progIt = 0;
        }
    }
    
    if (chassisNavTime > 0)
    {
        chassisBusy = 1;
        chassisNavTime--;
    }
    else
    {
        chassisBusy = 0;
        ChassisStop();
    }
}
