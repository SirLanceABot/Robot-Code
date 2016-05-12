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
#define BACK_DRIVE_ENCODER_A  1
#define BACK_DRIVE_ENCODER_B  2
#define FRONT_DRIVE_ENCODER_A   3
#define FRONT_DRIVE_ENCODER_B   4

//Steer Encoders
#define FRONT_ENCODER_A        5
#define FRONT_ENCODER_B        6
#define FRONT_ENCODER_INDEX    7
#define BACK_ENCODER_A         8
#define BACK_ENCODER_B         9
#define BACK_ENCODER_INDEX    10

//Shooter Encoder
#define SHOOTER_ENCODER_A	  11
#define SHOOTER_ENCODER_B	  12

//Switches
#define PRESSURE_SWITCH       13
#define SHOOTER_SWITCH        14

// --- PWM OUTPUTS ---
//Drive 
#define BACK_DRIVE_TALON      1
#define FRONT_DRIVE_TALON       2

//Steer
#define BACK_STEER_TALON       3
#define FRONT_STEER_TALON      4

//Shooter
#define BIG_SHOOTER_TALON      5
#define LITTLE_SHOOTER_TALON   6

//Scooper
#define SCOOPER_TALON	       7

// --- RELAY OUTPUTS ---
#define COMPRESSOR_RELAY       1
#define LED_RING_RELAY         2

//**********************************************
// DIGITAL SIDECAR MODULE 2
//**********************************************

// --- DIGITAL INPUTS ---

// --- RELAY OUTPUTS ---

//**********************************************
// ANALOG MODULE
//**********************************************

#define SCOOPER_POTENTIOMETER  1
#define GYRO_ANALOG_PORT       2
#define SONAR                  3

//**********************************************
// SOLENOID MODULE
//**********************************************

//Lifter
#define LIFTER_SOLENOID_OUT    1
#define LIFTER_SOLENOID_IN     2

//**********************************************
// XBOX JOYSTICK
//**********************************************

#define XBOX_CONTROLLER             1

#define XBOX_LEFT_X_AXIS            1
#define XBOX_LEFT_Y_AXIS            2
#define XBOX_TRIGGER_AXIS           3
#define XBOX_RIGHT_X_AXIS           4
#define XBOX_RIGHT_Y_AXIS           5

#define XBOX_A_BUTTON               1
#define XBOX_B_BUTTON               2
#define XBOX_X_BUTTON               3
#define XBOX_Y_BUTTON               4
#define XBOX_BACK_BUTTON            7
#define XBOX_START_BUTTON           8
#define XBOX_LEFT_STICK_PRESS       9
#define XBOX_RIGHT_STICK_PRESS     10
#define XBOX_LEFT_BUMPER		   11
#define XBOX_RIGHT_BUMPER		   12

//**********************************************
// LOGITECH JOYSTICK
//**********************************************

#define LOGITECH_CONTROLLER 2
#define LOGITECH_X_AXIS     1
#define LOGITECH_Y_AXIS     2
#define LOGITECH_Z_AXIS     3
#define LOGITECH_THROTTLE   4
#define LOGITECH_POV_X      5
#define LOGITECH_POV_Y      6

//**********************************************
// SWITCHBOARD
//**********************************************

#define SB_SWITCH_1               1
#define SB_SWITCH_2               2
#define SB_SWITCH_3               3
#define SB_SWITCH_4               4
#define SB_BIG_RED_BUTTON         5 
#define SB_SWITCH_5               6
#define SB_JOYSTICK_UP            7
#define SB_JOYSTICK_DOWN          8
#define SB_LITTLE_RED_BUTTON_LED  9
#define SB_SELECTOR_ONES         10
#define SB_SELECTOR_TWOS         11
#define SB_SELCETOR_FOURS        12
#define SB_BIG_RED_BUTTON_LED    13
#define SB_JOYSTICK_UP_LED       14
#define SB_JOYSTICK_DOWN_LED     15
#define SB_LITTLE_RED_BUTTON     16
#define SB_POTENTIOMETER_1        1
#define SB_POTENTIOMETER_2        2
#define SB_POTENTIOMETER_3        3

//**********************************************
// CAMERA
//**********************************************

#define Y_IMAGE_RES          240
#define VIEW_ANGLE            49
#define IP_ADDRESS "10.42.37.11"
#define HMIN                  30
#define HMAX                 230
#define SMIN                   0
#define SMAX                 255
#define LMIN                 200
#define LMAX                 255

#define BRIGHTNESS            50
#define MAXFPS                 7
#define RECTANGULARITY_LIMIT  40
#define ASPECT_RATIO_LIMIT    55
#define TAPE_WIDTH_LIMIT      50
#define VERTICAL_SCORE_LIMIT  50
#define LR_SCORE_LIMIT        50
#define AREA_MINIMUM         100
#define MAX_PARTICLES         10

//**********************************************
// CONSTANTS
//**********************************************

// General
#define PI                              3.1415926535897

// Scooper
#define SCOOPER_HOLD_BALL             670
#define SCOOPER_RELEASE_BALL          100
#define SCOOPER_PICK_UP_POSITION      300
#define SCOOPER_TOLERANCE               3

// Steering
#define TICKS_PER_WHEEL_REVOLUTION   2000
#define MIN_ANGLE_TOLERANCE             2
#define MID_ANGLE_TOLERANCE            10
#define MAX_ANGLE_TOLERANCE            20

// Shooter
#define SHOOTER_SHOOT_STOP            500
#define SHOOTER_RELOAD_STOP             5

// Drive
#define WHEEL_DIAMETER                  4.2
#define DRIVE_DISTANCE_PER_PULSE	    ((WHEEL_DIAMETER/12.0*PI)/(36.0/16.0*500.0))
#define SONAR_TOLERANCE                 3
#define SONAR_SHOOTING_DISTANCE        44

#endif
