/*
 * XboxJoystick.h
 *
 *  Created on: Jan 23, 2016
 *      Author: 18elafrenz
 */

#ifndef XBOXJOYSTICK_H_
#define XBOXJOYSTICK_H_

#include <memory>

class Joystick;

class XboxJoystick
{
public:
	enum ModificationType {kLinear, kSquared, kCubic, kCrawlWalkRun};
// declaring functions to get values from corresponding joystick

	//Constructor and destructor
	XboxJoystick(int);
	~XboxJoystick();

	//Button-related functions
	bool GetRawButton(int);
	bool GetA();
	bool GetB();
	bool GetX();
	bool GetY();
	bool GetStart();
	bool GetBack();
	bool GetLeftStickPress();
	bool GetRightStickPress();
	bool GetLeftBumper();
	bool GetRightBumper();

	//Joystick Axes
	float GetRawAxis(int axis);
	float GetModifiedAxis(int, ModificationType);
	float GetLeftTrigger();
	float GetRightTrigger();
	float GetLeftXAxis();
	float GetRightXAxis();
	float GetLeftYAxis();
	float GetRightYAxis();

	//D-pad functions and variables
	int GetPOV();

	const int UP = 0;
	const int RIGHT = 90;
	const int DOWN = 180;
	const int LEFT = 270;
	//Functions to set left and right rumble on XBox controller
	void SetLeftRumble(float kLeftRumble);
	void SetRightRumble(float kRightRumble);
// test
	void Test();


private:						// declare variable to receive values
//	Joystick* mJoystick;
	std::shared_ptr<Joystick> mJoystick;
};
#endif /* SRC_XBOXJOYSTICK_H_ */
