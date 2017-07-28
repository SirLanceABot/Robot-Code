// Author: Aaron Stoll, Andrew Measel
// Date: 26/1/17
// Revision History: Erin Lafren 1/31/2017: switched drive master talon with drive slave 2 talon
//					 Erin Lafrenz 2/4/17: switched the climb master with the climb slave

#ifndef CONSTANTS_H
#define CONSTANTS_H

// CAN ports
namespace CAN_PORT
{
static constexpr int LEFT_DRIVE_SLAVE_2 = 20;
static constexpr int LEFT_DRIVE_SLAVE_1 = 21;
static constexpr int LEFT_DRIVE_MASTER = 22;
static constexpr int SHOOTER = 29;
static constexpr int CLIMBER_MASTER = 32;
static constexpr int CLIMBER_SLAVE = 33;
static constexpr int RIGHT_DRIVE_SLAVE_2 = 33;
static constexpr int RIGHT_DRIVE_SLAVE_1 = 34;
static constexpr int RIGHT_DRIVE_MASTER = 35;
}

// Power Distribution Panel
namespace PDP_PORT
{
static constexpr int LEFT_DRIVE_SLAVE_2_TALON = 0;
static constexpr int LEFT_DRIVE_SLAVE_1_TALON = 1;
static constexpr int LEFT_DRIVE_MASTER_TALON = 2;
static constexpr int CLIMBER_SLAVE_TALON = 3;
//static constexpr int PORT_4 = 4;
//static constexpr int PORT_5 = 5;
static constexpr int RASBERRY_PI = 6;
static constexpr int LED_LIGHT = 7;
static constexpr int SHOOTER_SOLENOID = 8;
static constexpr int SHOOTER_TALON = 9;
//static constexpr int PORT_10 = 10;
//static constexpr int PORT_11 = 11;
static constexpr int CLIMBER_MASTER_TALON = 12;
static constexpr int RIGHT_DRIVE_SLAVE_2_TALON = 13;
static constexpr int RIGHT_DRIVE_SLAVE_1_TALON = 14;
static constexpr int RIGHT_DRIVE_MASTER_TALON = 15;
}

// Pneumatics control through CAN port
namespace PNEUMATICS_PORT
{
static constexpr int HIGH_SPEED = 1;
static constexpr int LOW_SPEED = 0;
static constexpr int CLOSE_GEAR = 2;
static constexpr int OPEN_GEAR = 3;
static constexpr int KICKER_OUT = 4;
static constexpr int KICKER_IN = 5;
//static constexpr int PORT_6 = 6;
//static constexpr int PORT_7 = 7;
}

//gyro board mount
namespace I2C_PORT
{
//static constexpr int SIX_DOF = I2C::Port::kMXP;
//static constexpr int SIX_DOF = I2C::Port::kOnboard;
}

//gyro constants
namespace GYRO
{
static constexpr int ADDRESS = 0x68;
//const int SAMPLE_RATE_FILTER = ITG3200::k1000Hz_10Hz;
//static constexpr double SENSITIVITY = 0.98 * ITG3200::SENSITIVITY_SCALE_FACTOR; // todo: sensitivity needs to be checked/calibrated; typically multiply default by a number near 1.0 to adjust accurately
}


// Digital RoboRio ports
namespace ROBORIO_DIGITAL_PORT
{
static constexpr int VISION_LIGHT_POWER = 10;
//static constexpr int PORT_11 = 11;
static constexpr int LIGHT_ONES_DIGIT = 12;
static constexpr int LIGHT_TWOS_DIGIT = 13;
static constexpr int LIGHT_FOURS_DIGIT = 14;
static constexpr int SHOOTER_SOLENOID = 18;
static constexpr int ROBOT_SELECT = 23;	// 0 = practice bot, 1 = competition bot
}

// Analog RoboRio ports
namespace ROBORIO_ANALOG_PORT
{
static constexpr int DISTANCE_SONAR = 4;
static constexpr int DISTANCE_IR = 5;
}

// Robot Constants
namespace ROBOT
{
//static constexpr int SPEED_GEAR_RATIO = ;
//static constexpr int POWER_GEAR_RATIO = ;
//static constexpr int WHEEL_CIRCUMFERENCE = ;
static constexpr double CLIMBER_GEAR_RATIO = 87.66;
static constexpr double HIGH_SPEED_GEAR_RATIO = 7.56;
static constexpr double LOW_SPEED_GEAR_RATIO = 22.67;
static constexpr int SHOOTER_GEAR_RATIO = 1;
}

// Autonomus Constants
namespace AUTONOMOUS
{
static constexpr double FINAL_SPEED = 0.8;
static constexpr double SIDE_SPEED = 0.8;
static constexpr double TURN_SPEED = 0.7;
static constexpr double BACKWARD_SPEED = -0.7;
static constexpr double VISION_SPEED = 0.4;
//static constexpr int GEAR_DROP_DISTANCE = ;
}

// Joysticks Constants
namespace JOYSTICK
{
static constexpr int PRIMARY_JOYSTICK = 0;
static constexpr int SECONDARY_JOYSTICK = 1;
static constexpr double DEADBAND = 0.1;

}

#endif
