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

#define SYS_TICK_TIME unsigned(1)
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





#define NAV_PROGRAM_SIZE 100


#define NR_OF_TASKS 5

#define LINE_SENSOR_NR_OF 5

#define SENS_LINE_FAR_LEFT_ID 0
#define SENS_LINE_LEFT_ID 1
#define SENS_LINE_CENTER_ID 2
#define SENS_LINE_RIGHT_ID 3
#define SENS_LINE_FAR_RIGHT_ID 4


#define DIST_BUFF_SIZE 3

#define INTIME_DISTANCE (50.0)
#define MAX_DUR ((INTIME_DISTANCE * 2.0) / 0.034)

#define DISTANCE_SENSOR_NR_OF 3


#define SENS_US_LEFT_ID 0
#define SENS_US_CENTER_ID 1
#define SENS_US_RIGHT_ID 2

#define BUMPER_NR_OF 2
#define SENS_BUMPER_LEFT_ID 0
#define SENS_BUMPER_RIGHT_ID 1


#define MORSE_FRQ 440
#define MORSE_DOT_DUR (100)
#define MORSE_DASH_DUR (MORSE_DOT_DUR * 3)
#define MORSE_PAUSE (MORSE_DASH_DUR)


#define DISTANCE_MIN 7
#define DISTANCE_MAX 15

#define APP_STATE_MANUAL 1
#define APP_STATE_AUTO_FOLLOW 2
#define APP_STATE_AUTO_TUNEL 3
#endif
