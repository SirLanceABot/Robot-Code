#ifndef LHS_JOYSTICK_H
#define LHS_JOYSTICK_H
using namespace std;

class LHSJoystick
{
public:
	LHSJoystick(int);
	~LHSJoystick();
	float GetRawAxis(int, int);
	bool GetRawButton(int);
	float GetMagnitude();
	int GetPOV();
private:
	Joystick* mStick;
};

LHSJoystick::LHSJoystick(int joystickPort)
{
	mStick = new Joystick(joystickPort);
}

LHSJoystick::~LHSJoystick()
{
	delete mStick;
	mStick = NULL;
}

float LHSJoystick::GetRawAxis(int Axis, int mode)
{
/*	if ((Axis == XBOX_RIGHT_Y_AXIS || Axis == XBOX_LEFT_Y_AXIS) && mode == DRIVE_MODE)
	{
		return mStick->GetRawAxis(Axis);
	}*/
	return mStick->GetRawAxis(Axis);
}

bool LHSJoystick::GetRawButton(int Button)
{
	return mStick->GetRawButton(Button);
}

float LHSJoystick::GetMagnitude()
{
	return mStick->GetMagnitude();
}

int LHSJoystick::GetPOV()
{
	return mStick->GetPOV();
}
#endif /* SRC_LHSJOYSTICK_ */
