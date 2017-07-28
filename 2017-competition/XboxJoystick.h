// Authors: Brady Dutton and Maanasa Bommineni and Luke Reuter
// Date: 1/16/2017

// Revision History:
// 30/1/17 Aaron Stoll: Deadzone added/SetDeadzone

#ifndef XBOXJOYSTICK_H
#define XBOXJOYSTICK_H

#include <memory>

namespace frc
{
	class Joystick;
}
class XboxJoystick
{
public:
	// Buttons
	static constexpr int A_BUTTON = 1;
	static constexpr int B_BUTTON = 2;
	static constexpr int X_BUTTON = 3;
	static constexpr int Y_BUTTON = 4;
	static constexpr int LEFT_BUMPER = 5;
	static constexpr int RIGHT_BUMPER = 6;
	static constexpr int BACK_BUTTON = 7;
	static constexpr int START_BUTTON = 8;
	static constexpr int LEFT_STICK_PRESS = 9;
	static constexpr int RIGHT_STICK_PRESS = 10;
	static constexpr int LEFT_X_AXIS = 0;
	static constexpr int LEFT_Y_AXIS = 1;
	static constexpr int RIGHT_X_AXIS = 4;
	static constexpr int RIGHT_Y_AXIS = 5;
	static constexpr int LEFT_TRIGGER = 2;
	static constexpr int RIGHT_TRIGGER = 3;

	XboxJoystick(int port);

	// Axis
	bool GetA();
	bool GetB();
	bool GetX();
	bool GetY();
	bool GetLeftBumper();
	bool GetRightBumper();
	bool GetBackButton();
	bool GetStartButton();
	bool GetLeftStickPress();
	bool GetRightStickPress();

	double GetLeftX();
	double GetLeftY();
	double GetRightX();
	double GetRightY();
	double GetLeftTrigger();
	double GetRightTrigger();

	int GetPOV();

	void Print();

	// Rumble
	void SetRightRumble(double value);
	void SetLeftRumble(double value);
	void SetDeadZone(double deadzone);


private:
	std::shared_ptr<frc::Joystick> mXbox;
	double mDeadzone;
	bool GetButton(int button);
	double GetAxis(int axis);
};



#endif /* SRC_XBOXJOYSTICK_H_ */
