#ifndef XBOXJOYSTICK_H
#define XBOXJOYSTICK_H

class XBoxJoystick
{
public:
	XBoxJoystick(void);
	~XBoxJoystick(void);
	
	void JoystickTest(void);
	void WaitForX(void);
	void WaitForButton(int);
	
	double GetLeftJoystickAngle(void);
	double GetRightJoystickAngle(void);
	double GetLeftJoystickSpeed(void);
	double GetLeftJoystickSpeedSquared(void);
	double GetLeftJoystickSpeedCubed(void);
	double GetAxis(int);
	
	int GetXBoxButton(int);

private:
	Joystick* mXBoxJoystick;
};

XBoxJoystick::XBoxJoystick(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
	
	mXBoxJoystick = new Joystick(XBOX_CONTROLLER);
}

XBoxJoystick::~XBoxJoystick(void)
{
	delete mXBoxJoystick;
	mXBoxJoystick = NULL;
}

void XBoxJoystick::JoystickTest(void)
{
	double x, y, angle;
	while(mXBoxJoystick->GetRawButton(XBOX_BACK_BUTTON)!= 1)
	{
		x = mXBoxJoystick->GetRawAxis(XBOX_LEFT_X_AXIS);
		y = -mXBoxJoystick->GetRawAxis(XBOX_LEFT_Y_AXIS);

		angle = GetLeftJoystickAngle();

		//printf("X = %f  Y = %f  Angle = %f\n", x, y,  angle);

		Wait(0.1);
	}
}

void XBoxJoystick::WaitForX(void)
{
	printf("Press X\n");
	while(mXBoxJoystick->GetRawButton(XBOX_X_BUTTON) != 1)
	{
		Wait(.05);	
	}	
	
	while(mXBoxJoystick->GetRawButton(XBOX_X_BUTTON) == 1)
	{
		Wait(.05);
	}
}

double XBoxJoystick::GetLeftJoystickAngle(void)
{
	double x, y, angle;

	x = mXBoxJoystick->GetRawAxis(XBOX_LEFT_X_AXIS);
	y = -mXBoxJoystick->GetRawAxis(XBOX_LEFT_Y_AXIS);

	angle = (atan2(y,x) * (180.0 / PI));

	if(angle < 0.0)
	{
		angle = angle + 360.0;
	}

	return angle;
}

double XBoxJoystick::GetRightJoystickAngle(void)
{
	double x, y, angle;

	x = mXBoxJoystick->GetRawAxis(XBOX_RIGHT_X_AXIS);
	y = -mXBoxJoystick->GetRawAxis(XBOX_RIGHT_Y_AXIS);

	angle = (atan2(y,x) * (180.0 / PI));

	if(angle < 0.0)
		angle = angle + 360.0;

	return angle;
}

double XBoxJoystick::GetLeftJoystickSpeed(void)
{
	double x, y, speed;

	x = mXBoxJoystick->GetRawAxis(XBOX_LEFT_X_AXIS);
	y = -mXBoxJoystick->GetRawAxis(XBOX_LEFT_Y_AXIS);

	speed = sqrt(x * x + y * y);

	return speed;
}

double XBoxJoystick::GetLeftJoystickSpeedSquared(void)
{
	double speed = GetLeftJoystickSpeed();
	
	return fabs(speed) * speed;
}

double XBoxJoystick::GetLeftJoystickSpeedCubed(void)
{
	double speed = GetLeftJoystickSpeed();
	
	return speed * speed * speed;
}

int XBoxJoystick::GetXBoxButton(int button)
{
	if(mXBoxJoystick->GetRawButton(button))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

double XBoxJoystick::GetAxis(int axis)
{
	if(axis == XBOX_LEFT_Y_AXIS || axis == XBOX_RIGHT_Y_AXIS)
	{
		return -mXBoxJoystick->GetRawAxis(axis);	
	}
	else
	{
		return mXBoxJoystick->GetRawAxis(axis);	
	}
}

void XBoxJoystick::WaitForButton(int button)
{
	while(mXBoxJoystick->GetRawButton(button) != 1)
	{
		Wait(.001);	
	}	
	
	while(mXBoxJoystick->GetRawButton(button) == 1)
	{
		Wait(.001);
	}
}

#endif
