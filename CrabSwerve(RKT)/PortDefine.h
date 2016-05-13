#ifndef PORTDEFINE_H
#define PORTDEFINE_H


//**********************************************
// DIGITAL SIDECAR MODULES
//**********************************************
#define MODULE_1               1
#define MODULE_2               2


//**********************************************
// DIGITAL SIDECAR MODULE 1
//**********************************************

// --- DIGITAL INPUTS ---
//Drive Encoders

#define LEFT_DRIVE_ENCODER_A   1
#define LEFT_DRIVE_ENCODER_B   2
#define RIGHT_DRIVE_ENCODER_A  3
#define RIGHT_DRIVE_ENCODER_B  4

//Steer Encoders
#define FRONT_ENCODER_A        5
#define FRONT_ENCODER_B        6
#define FRONT_ENCODER_INDEX    7
#define BACK_ENCODER_A         8
#define BACK_ENCODER_B         9
#define BACK_ENCODER_INDEX    10

// --- PWM OUTPUTS ---
//Drive 
#define RIGHT_DRIVE_TALON      1
#define LEFT_DRIVE_TALON       2

//Steer
#define FRONT_STEER_TALON      4
#define BACK_STEER_TALON       3

//**********************************************
// ANALOG MODULE
//**********************************************

#define GYRO_ANALOG_PORT       1
#define GYRO_TEMPERATURE_PORT  2
#define BATTERY_VOLTAGE        3

//**********************************************
// XBOX JOYSTICK
//**********************************************

#define XBOX_CONTROLLER        1

#define LEFT_X_AXIS            1
#define LEFT_Y_AXIS            2
#define TRIGGER_AXIS           3
#define RIGHT_X_AXIS           5
#define RIGHT_Y_AXIS           6

#define A_BUTTON               1
#define B_BUTTON               2
#define X_BUTTON               3
#define Y_BUTTON               4
#define LEFT_BUMPER            5
#define RIGHT_BUMPER           6
#define BACK_BUTTON            7
#define START_BUTTON           8
#define LEFT_STICK_PRESS       9
#define RIGHT_STICK_PRESS     10

//**********************************************
// CONSTANTS
//**********************************************

#define TICKS_PER_WHEEL_REVOLUTION   2000
#define PI                              3.1415926535897
#define MIN_ANGLE_TOLERANCE             5
#define MID_ANGLE_TOLERANCE            25
#define MAX_ANGLE_TOLERANCE            60
#define RATE_TOLERANCE                  2
#define WHEEL_DIAMETER                4.2
#define GYRO_SENSITIVITY_AJUSTMENT 87./90.

#endif
