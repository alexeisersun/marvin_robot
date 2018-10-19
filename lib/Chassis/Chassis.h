#ifndef CHASSIS_H_
#define CHASSIS_H_

#include <Arduino.h>
#include <RobotDefinitions.h>
#include <L298N.h>


//create a Left motor instance
L298N motorLeft(L298N_ENA, L298N_IN1, L298N_IN2);
//create a Right motor instance
L298N motorRight(L298N_ENB, L298N_IN3, L298N_IN4);

extern int nav_speed = NAV_SPEED;
extern char chassisBusy = 0;
extern volatile int chassisNavTime = 0;
extern char chassisProgram = 0;
extern int progIt = 0;

extern char nav_program[NAV_PROGRAM_SIZE] = "FLFLFLFL";
extern char log_program[NAV_PROGRAM_SIZE] = {""};
extern int log_program_it = 0;

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