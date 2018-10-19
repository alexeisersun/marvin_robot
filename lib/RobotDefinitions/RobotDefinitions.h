#ifndef ROBOT_DEFINITIONS_H_
#define ROBOT_DEFINITIONS_H_

#define CTRL_SERIAL Serial
#define MON_SERIAL Serial

// Binary Sensor Pin Mapping
#define SENS_SOUND_DET_PIN A6

#define SENS_BUMPER_LEFT_PIN 48
#define SENS_BUMPER_RIGHT_PIN 46

// Analog sensor Pin Mapping
#define SENS_LINE_FAR_LEFT A11
#define SENS_LINE_LEFT A12
#define SENS_LINE_CENTER A13
#define SENS_LINE_RIGHT A14
#define SENS_LINE_FAR_RIGHT A15

// Distance sensor Pinmap

#define SENS_US_TRIG_LEFT_PIN 38
#define SENS_US_ECHO_LEFT_PIN 44

#define SENS_US_TRIG_CENTER_PIN 36
#define SENS_US_ECHO_CENTER_PIN 42

#define SENS_US_TRIG_RIGHT_PIN 34
#define SENS_US_ECHO_RIGHT_PIN 40

// Motor Pin Mapping
#define L298N_IN1 22
#define L298N_IN2 24
#define L298N_ENA 2

#define L298N_IN3 26
#define L298N_IN4 28
#define L298N_ENB 3

#define SYS_TICK_TIME 1
#define CHASSIS_REQ 50
//====================================================
// Chassis Driver
//====================================================
#define NAV_SPEED 200
#define NAV_SPEED_MAX 255
#define NAV_SPEED_COR_L (10)
#define NAV_SPEED_COR_R (0)

#define NAV_WALK_STEP (500/( CHASSIS_REQ * SYS_TICK_TIME))
#define NAV_TURN_ANGLE (180/( CHASSIS_REQ * SYS_TICK_TIME))
#define PROG_WALK_STEP (500 /( CHASSIS_REQ * SYS_TICK_TIME))
#define PROG_TURN_ANGLE (180 /( CHASSIS_REQ * SYS_TICK_TIME))

#endif
