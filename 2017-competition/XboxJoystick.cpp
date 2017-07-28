// Author: Brady Dutton, Maanasa Bommineni
// 1/16/17

// Revision History:
// 30/1/17 Aaron Stoll: Deadzone/SetDeadzone added
// 2/09/17 Lady Button, Maanasa Bommineni, Puke Tooter the Scooter

#include "XboxJoystick.h"
#include "WPILib.h"

// Get values from buttons and axis
XboxJoystick::XboxJoystick (int port):
	mXbox(std::make_shared<frc::Joystick>(port)),
	mDeadzone(0.2)
{
	std::cout << "[XboxJoystick]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

// Get button
bool XboxJoystick::GetButton(int button)
{
	return mXbox->GetRawButton(button);
}

// Get value of A button
bool XboxJoystick::GetA()
{
	return GetButton(A_BUTTON);
}

// Get value of B button
bool XboxJoystick::GetB()
{
	return GetButton(B_BUTTON);
}

// Get value of X button
bool XboxJoystick::GetX()
{
	return GetButton(X_BUTTON);
}

// Get value of Y button
bool XboxJoystick::GetY()
{
	return GetButton(Y_BUTTON);
}

// Get value off Left Bumper
bool XboxJoystick::GetLeftBumper()
{
	return GetButton(LEFT_BUMPER);
}

// Get value off Right Bumper
bool XboxJoystick::GetRightBumper()
{
	return GetButton(RIGHT_BUMPER);
}

// Get value of Back Button
bool XboxJoystick::GetBackButton()
{
	return GetButton(BACK_BUTTON);
}

// Get value of Start Button
bool XboxJoystick::GetStartButton()
{
	return GetButton(START_BUTTON);
}

// Check if the left stick is pressed
bool XboxJoystick::GetLeftStickPress()
{
	return GetButton(LEFT_STICK_PRESS);
}

// Check if the right stick is pressed
bool XboxJoystick::GetRightStickPress()
{
	return GetButton(RIGHT_STICK_PRESS);
}

// Get value of the axis and implement deadzone
double XboxJoystick::GetAxis(int axis)
{
	double value;
	value = mXbox->GetRawAxis(axis);

	if (value > -mDeadzone && value < mDeadzone)
	{
		value = 0.0;
	}

	return value;
}

// Get the value of the left x axis
double XboxJoystick::GetLeftX()
{
	return GetAxis(LEFT_X_AXIS);
}

// Get the value of the left y axis
double XboxJoystick::GetLeftY()
{
	return -GetAxis(LEFT_Y_AXIS);
}

// Get the value of the right x axis
double XboxJoystick::GetRightX()
{
	return GetAxis(RIGHT_X_AXIS);
}

// Get the value of the right y axis
double XboxJoystick::GetRightY()
{
	return -GetAxis(RIGHT_Y_AXIS);
}

// Gets the value of the left trigger
double XboxJoystick::GetLeftTrigger()
{
	return GetAxis(LEFT_TRIGGER);
}

// Gets the value of the right trigger
double XboxJoystick::GetRightTrigger()
{
	return GetAxis(RIGHT_TRIGGER);
}

// Sets the deadzone
void XboxJoystick::SetDeadZone(double deadzone)
{
	this->mDeadzone = deadzone;
}

// Gets the value from the POV
int XboxJoystick::GetPOV()
{
	return mXbox->GetPOV();
}

// Test all the buttons and axis
void XboxJoystick::Print()
{
	double rightTrigger = GetRightTrigger();
	double leftTrigger = GetLeftTrigger();
	double rightX = GetRightX();
	double rightY = GetRightY();
	double leftX = GetLeftX();
	double leftY = GetLeftY();

	if (rightTrigger != 0)
	{
		std::cout << "Right Trigger: " << rightTrigger << std::endl;
	}
	if (leftTrigger != 0)
	{
		std::cout << "Left Trigger: " << leftTrigger << std::endl;
	}


	if (rightX != 0 || rightY != 0)
	{
		std::cout << "RightX: " << rightX << " RightY: " << rightY << std::endl;
	}

	if (leftX != 0 || leftY != 0)
	{
		std::cout << "LeftX: " << leftX << " LeftY: " << leftY << std::endl;
	}

	if (GetA() == true)
	{
		std::cout << "A is pressed" << std::endl;
	}

	if (GetB() == true)
	{
		std::cout << "B is pressed" << std::endl;
	}

	if (GetX() == true)
	{
		std::cout << "X is pressed" << std::endl;
	}

	if (GetY() == true)
	{
		std::cout << "Y is pressed" << std::endl;
	}

	if (GetLeftBumper() == true)
	{
		std::cout << "Left Bumper is pressed" << std::endl;
		SetLeftRumble(1.0);
	}
	else
	{
		SetLeftRumble(0.0);
	}

	if (GetRightBumper() == true)
	{
		std::cout << "Right Bumper is pressed" << std::endl;
		SetRightRumble(1.0);
	}
	else
	{
		SetRightRumble(0.0);
	}

	if (GetBackButton() == true)
	{
		std::cout << "Back Button is pressed" << std::endl;
	}

	if (GetStartButton() == true)
	{
		std::cout << "Start Button is pressed" << std::endl;
	}

	if (GetLeftStickPress() == true)
	{
		std::cout << "Left Stick is pressed" << std::endl;
	}

	if (GetRightStickPress() == true)
	{
		std::cout << "Right Stick is pressed" << std::endl;
	}

	if (GetPOV() != -1)
	{
		std::cout << "POV: "<< GetPOV() << std::endl;
	}
}

// Set left rumble
void XboxJoystick::SetLeftRumble(double value)
{
	mXbox->SetRumble(Joystick::kLeftRumble, value);
}

// Set right rumble
void XboxJoystick::SetRightRumble(double value)
{
	mXbox->SetRumble(Joystick::kRightRumble, value);
}
