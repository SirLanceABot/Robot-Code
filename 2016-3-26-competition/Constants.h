/*
 * Constants.h
 *
 *  Created on: Jan 23, 2016
 *      Author: 18elafrenz
 */


#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "WPIlib.h"
#include "ITG-3200.h"

namespace XBOX
{
// declaring buttons as constants from the xbox joystick
const int DRIVER_PORT = 0;
const int A_BUTTON = 1;
const int B_BUTTON = 2;
const int X_BUTTON = 3;
const int Y_BUTTON = 4;
const int LEFT_BUMPER_BUTTON = 5;
const int RIGHT_BUMPER_BUTTON = 6;
const int BACK_BUTTON = 7;
const int START_BUTTON = 8;
const int LEFT_STICK_PRESS_BUTTON = 9;
const int RIGHT_STICK_PRESS_BUTTON = 10;
const int NODE_ID = 1;

// declaring axes as constants from joystick
const int LEFT_X_AXIS = 0;
const int LEFT_Y_AXIS = 1;
const int LEFT_TRIGGER_AXIS = 2;
const int RIGHT_TRIGGER_AXIS = 3;
const int RIGHT_X_AXIS = 4;
const int RIGHT_Y_AXIS = 5;

const int POV_UP = 0;
const int POV_RIGHT = 90;
const int POV_DOWN = 180;
const int POV_LEFT = 270;
}

namespace LOGITECH
{
//declaring buttons as constants from the logitech joystick
const int TRIGGER = 1;
const int BUTTON_2 = 2;
const int BUTTON_3 = 3;
const int BUTTON_4 = 4;
const int BUTTON_5 = 5;
const int BUTTON_6 = 6;
const int BUTTON_7 = 7;
const int BUTTON_8 = 8;
const int BUTTON_9 = 9;
const int BUTTON_10 = 10;
const int BUTTON_11 = 11;
const int BUTTON_12 = 12;

//declaring axes as constants from the logitech joystick
const int X_AXIS = 0;
const int Y_AXIS = 1;
const int Z_AXIS = 2;
const int THROTTLE_AXIS = 3;
}

namespace CAN_PORT
{
const int POWER_DISTRIBUTION_PANEL = 0;

// TALONS
const int FRONT_LEFT = 0;
const int REAR_LEFT = 2;
const int FRONT_RIGHT = 1;
const int REAR_RIGHT = 3;
const int RAISE_SHOOTER = 4;
const int SHOOTER_LEFT_WHEEL = 5;
const int SHOOTER_RIGHT_WHEEL = 6;
const int SHOOTER_PUSH = 7;

const int LEFT_WINCH = 8;
const int RIGHT_WINCH = 9;
const int ARM_PIVOT = 10;
const int ARM_EXTEND = 11;
}

namespace DIO_PORT //0-9, expansion board 10+
{
const int LIGHT_RING = 10;
const int VERTICAL_LIMIT = 11;
const int ROBOT_SELECTOR = 13; //0 = practice, 1 = competition
}

namespace I2C_PORT
{
const I2C::Port SIX_DOF = I2C::Port::kMXP;
//const I2C::Port SIX_DOF = I2C::Port::kOnboard;
}

namespace DRIVE_TRAIN
{
const float PI = 3.1415927;
const float WHEEL_CIRCUMFERENCE = 9.42477;
const float SLOW_ROTATE = .8;
const float TICKS_PER_REVOLUTION = 1000;
const float REVOLUTION_FOR_180_TURN = 6.66667;
}

namespace AUTONOMOUS
{
const float ROTATE_SPEED = 0.58;
const float AUTO_DRIVE_TIME = 3.5;

const int SHOOTER_ANGLE_UP = 0;
const int SHOOTER_ANGLE_DOWN = 5350;
const int SHOOTER_ANGLE_VISION = 2950; //x
const int SHOOTER_ANGLE_SHOOT = 1800; //x
const int SHOOTER_ANGLE_JAM_SHOT = 470;

const int SHOOTER_ANGLE_TOLERANCE = 27;

const int SHOOTER_PUSHER_REVERSE = 0;
const int SHOOTER_PUSHER_FORWARD = 160;
}

namespace GYRO
{
const int ADDRESS = ITG3200::ITG3200_I2C_ADDR;
const int SAMPLE_RATE_FILTER = ITG3200::k1000Hz_10Hz;
const float SENSITIVITY = 0.99 * ITG3200::SENSITIVITY_SCALE_FACTOR; // todo: sensitivity needs to be checked/calibrated; typically multiple default by a number near 1.0 to adjust accurately
}

namespace SWITCHBOARD
{
const int SWITCHBOARD_PORT = 0;
// Declaring constants for the switchboard
const int SWITCH_0 = 1;
const int SWITCH_1 = 2;
const int SWITCH_2 = 3;
const int SWITCH_3 = 4;
const int SWITCH_4 = 5;
const int SMALL_BUTTON = 12;
const int JOYSTICK_UP = 6;
const int JOYSTICK_DOWN = 7;
const int BIG_BUTTON = 13;

const int POT_0 = 0;
const int POT_1 = 1;
const int POT_2 = 2;

const int SMALL_BUTTON_LED = 3;
const int JOYSTICK_UP_LED = 2;
const int JOYSTICK_DOWN_LED = 1;
const int BIG_BUTTON_LED = 4;

const int COUNTER_PORT_8 = 8;
const int COUNTER_PORT_9 = 9;
const int COUNTER_PORT_10 = 10;
}

namespace PDP_PORTS
{
const int LEFT_DRIVE_MASTER = 0;             //These are what will be plugged into the ports for the PDP Panel
const int LEFT_DRIVE_FOLLOW = 1;
const int LEFT_SHOOT_MASTER = 2;
const int LEFT_WINCH_MASTER = 3;
const int SHOOTER_LIFT = 4;
const int PUSHER = 5;
const int LIGHT_RING_POWER = 7;
const int RIGHT_DRIVE_MASTER = 15;
const int RIGHT_DRIVE_FOLLOW = 14;
const int RIGHT_SHOOT_MASTER = 13;
const int RIGHT_WINCH_MASTER = 12;
const int ARM_PIVOT = 11;
const int ARM_EXTEND = 10;
const int MINI_CIM_MAX_AMPS = 10;       //From here down PDP Constants are the limits in amps for different things hooked to the PDP Panel
const int BOSCH_MAX_AMPS = 10;
const int WINDOW_MAX_AMPS = 10;
const int CIM_MAX_AMPS = 10;
const int PLANETARY_MAX_AMPS = 10;
const int LIGHT_RING_MAX_AMPS = 5;
const int TOTAL_PDP_LIMIT = 100;
}


#endif /* SRC_CONSTANTS_H_ */
