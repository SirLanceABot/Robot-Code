#ifndef PORTDEFINE_H
#define PORTDEFINE_H

//**********************************************
// SmartDashboard
//**********************************************
#define WORK           0
#define PRODUCTIVITY   0
#define ACCOMPLISHMENT 0
// --- Macros ---
#define DBSTR(x,y) SmartDashboard::PutString("DB/String "#x, y)
#define DBLED(x,y) SmartDashboard::PutBoolean("DB/LED "#x,y)
#define DBSLIDER(x,y) SmartDashboard::PutNumber("DB/Slider "#x,y)
#define DBBUTTON(x,y) SmartDashboard::PutBoolean("DB/Button "#x,y)

#define DBGSTR(x) SmartDashboard::GetString("DB/String "#x)
#define DBGLED(x) SmartDashboard::GetBoolean("DB/LED "#x)
#define DBGSLIDER(x) SmartDashboard::GetNumber("DB/Slider "#x)
#define DBGBUTTON(x) SmartDashboard::GetBoolean("DB/Button "#x)

//**********************************************
// DIGITAL RoboRIO
//**********************************************

// --- PWM OUTPUTS ---
//Drive 
 //Drop 6
#define LEFT_DRIVE_TALON       0
#define RIGHT_DRIVE_TALON      1
 //Mecanum
#define REAR_RIGHT_TALON       0
#define FRONT_RIGHT_TALON      1
#define FRONT_LEFT_TALON       2
#define REAR_LEFT_TALON        3
//Other
#define ELEVATOR_TALON         2 //wrong if mecanum
#define ARM_TALON              3 //wrong if mecanum
#define HOOK_TALON             4

// --- DIGITAL INPUTS ---
#define ELEV_BOTTOM_LIMIT_SWITCH_PORT    0
#define ELEV_TOP_LIMIT_SWITCH_PORT       1
#define HALL_EFFECT_OUT                  2
#define HALL_EFFECT_IN                   3
#define SIXTEEN_POINT_SWITCH_ONES        18
#define SIXTEEN_POINT_SWITCH_TWOS		 19
#define SIXTEEN_POINT_SWITCH_FOURS		 20
#define SIXTEEN_POINT_SWITCH_EIGHTS		 21

// --- ENCODER PORTS ---
#define LEFT_ENCODER_B         10
#define LEFT_ENCODER_A         11
#define RIGHT_ENCODER_B        12
#define RIGHT_ENCODER_A        13
#define ARM_ENCODER_B          14
#define ARM_ENCODER_A          15

// --- RELAY OUTPUTS ---
#define RING_LIGHT_RELAY        0

// --- ANALOG INPUTS ---
#define GYRO_ANALOG_PORT        0
#define IR_ANALOG_PORT          2
#define SONAR_ANALOG_PORT       3 //no longer on robot
#define STRING_POT_PORT         4
#define CALIBRATE_SONAR_PORT    7

//**********************************************
// XBOX JOYSTICK
//**********************************************

#define XBOX_CONTROLLER             0

#define XBOX_LEFT_X_AXIS            0
#define XBOX_LEFT_Y_AXIS            1
#define XBOX_LEFT_TRIGGER_AXIS      2
#define XBOX_RIGHT_TRIGGER_AXIS     3
#define XBOX_RIGHT_X_AXIS           4
#define XBOX_RIGHT_Y_AXIS           5

#define XBOX_A_BUTTON               1
#define XBOX_B_BUTTON               2
#define XBOX_X_BUTTON               3
#define XBOX_Y_BUTTON               4
#define XBOX_LEFT_BUMPER		    5
#define XBOX_RIGHT_BUMPER		    6
#define XBOX_BACK_BUTTON            7
#define XBOX_START_BUTTON           8
#define XBOX_LEFT_STICK_PRESS       9
#define XBOX_RIGHT_STICK_PRESS     10

//**********************************************
// LOGITECH JOYSTICK
//**********************************************

#define LOGITECH_CONTROLLER  1
#define LOGITECH_X_AXIS      1
#define LOGITECH_Y_AXIS      2
#define LOGITECH_Z_AXIS      3
#define LOGITECH_THROTTLE    4
#define LOGITECH_POV_X       5
#define LOGITECH_POV_Y       6

//**********************************************
// SWITCHBOARD
//**********************************************

#define SWITCHBOARD_CONTROLLER              2
#define SWITCHBOARD_BUTTON_1                1
#define SWITCHBOARD_BUTTON_2                2
#define SWITCHBOARD_BUTTON_3                3
#define SWITCHBOARD_BUTTON_4                4
#define SWITCHBOARD_BUTTON_5                5
#define SWITCHBOARD_JOYSTICK_UP             6
#define SWITCHBOARD_JOYSTIC_DOWN            7
#define SWITCHBOARD_COUNTER_ONES            8
#define SWITCHBOARD_COUNTER_TWOS            9
#define SWITCHBOARD_COUNTER_FOURS           10
#define	SWITCHBOARD_LITTLE_RED_BUTTON       12
#define	SWITCHBOARD_BIG_RED_BUTTON          13
#define SWITCHBOARD_POT_1                   0
#define SWITCHBOARD_POT_2                   1
#define SWITCHBOARD_POT_3                   2
#define SWITCHBOARD_JOYSTICK_DOWN_LED       1
#define SWITCHBOARD_JOYSTICK_UP_LED         2
#define SWITCHBOARD_LITTLE_RED_BUTTON_LED   3
#define SWITCHBOARD_BIG_RED_BUTTON_LED      4


//**********************************************
// CAMERA
//**********************************************

// --- Constants ---
//Tape and Tote
#define LONG_RATIO        2.22 //Tote long side = 26.9 / Tote height = 12.1 = 2.22
#define SHORT_RATIO       1.40 //Tote short side = 16.9 / Tote height = 12.1 = 1.4
#define SCORE_MIN         75.0 //Minimum score to be considered a tote
#define VIEW_ANGLE        60.0 //View angle for camera, set to Axis m1011 by default, 64 for m1013, 51.7 for 206, 52 for HD3000 square, 60 for HD3000 640x480
//Tape
#define AREA_MINIMUM_TAPE 0.5 //Default Area minimum for particle as a percentage of total image area
//Tote
#define AREA_MINIMUM_TOTE 2.0 //Default Area minimum for particle as a percentage of total image area

//**********************************************
// CONSTANTS
//**********************************************

// --- General ---
#define PI                             3.1415926535897

// --- Gyro ---
#define SENSITIVITY_ADJUSTMENT         87.0/90.0

// --- String Pot ---
#define STRING_POT_UNITS        6.96
#define STRING_POT_OFFSET       -0.52

// --- Steering ---
#define MIN_ANGLE_TOLERANCE            8
#define MID_ANGLE_TOLERANCE            10
#define MAX_ANGLE_TOLERANCE            20

// --- Drive ---
#define WHEEL_DIAMETER                  6.0
#define DRIVE_DISTANCE_PER_PULSE	    ((WHEEL_DIAMETER*PI)/(250.0))
#define SONAR_TOLERANCE                 3
#define SONAR_DIST_FROM_FRONT           1
#define SONAR_INCOMING_VOLTAGE          4.85
#define DRIVE_MODE                      0
#define ELEVATOR_MODE                   1

#endif
