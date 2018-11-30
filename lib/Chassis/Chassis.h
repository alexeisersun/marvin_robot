#ifndef CHASSIS_H_
#define CHASSIS_H_

#include <Arduino.h>
#include <RobotDefinitions.h>
#include <L298N.h>

//create a Left motor instance
extern L298N motorLeft;
//create a Right motor instance
extern L298N motorRight;


extern int nav_speed;
extern char chassisBusy;
extern volatile int chassisNavTime;
extern char chassisProgram;
extern int progIt;

extern char nav_program[NAV_PROGRAM_SIZE];
extern char log_program[NAV_PROGRAM_SIZE];
extern int log_program_it;

void ChassisNavSpeedUp(void);
void ChassisNavSpeedInit(void);
void ChassisStop(void);
void ChassisStepFWD(int steps);
void ChassisStepBWD(int steps);
void ChassisTurnLeft(int angle);
void ChassisTurnRight(int angle);
void ChassisNavProgram(char program[]);
void ChassisTask_Run(void);

#endif