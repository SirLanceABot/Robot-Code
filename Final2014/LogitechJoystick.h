#ifndef LOGITECH_JOYSTICK_H
#define LOGITECH_JOYSTICK_H

class LogitechJoystick
{
public:
	LogitechJoystick(void);
	~LogitechJoystick(void);

	double GetAxis(int);
	int GetPOV(int);
	double GetJoystickAngle(void);
	double GetJoystickSpeed(void);
	double GetJoystickSpeedSquared(void);
	double GetJoystickSpeedCubed(void);

	int GetLogitechButton(int);

private:
	Joystick* mJoystick;
};

LogitechJoystick::LogitechJoystick(void) 
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mJoystick = new Joystick(LOGITECH_CONTROLLER);
}

LogitechJoystick::~LogitechJoystick(void)
{
	delete mJoystick;
	mJoystick = NULL;
}

double LogitechJoystick::GetAxis(int axis)
{
	if(axis == LOGITECH_Y_AXIS || axis == LOGITECH_THROTTLE)
	{
		return (-mJoystick->GetRawAxis(axis));
	}
	else
	{
		return (mJoystick->GetRawAxis(axis));
	}
}

int LogitechJoystick::GetPOV(int axis)
{
	if(axis == LOGITECH_POV_Y)
	{
		return((int)-mJoystick->GetRawAxis(axis));
	}
	else
	{
		return((int)mJoystick->GetRawAxis(axis));
	}
}

double LogitechJoystick::GetJoystickAngle(void)
{
	double x, y, angle;

	x = mJoystick->GetRawAxis(LOGITECH_X_AXIS);
	y = -mJoystick->GetRawAxis(LOGITECH_Y_AXIS);

	angle = (atan2(y,x) * (180.0 / PI));

	if(angle < 0.0)
	{
		angle = angle + 360.0;
	}

	return angle;
}

double LogitechJoystick::GetJoystickSpeed(void)
{
	double x, y, speed;

	x = mJoystick->GetRawAxis(LOGITECH_X_AXIS);
	y = -mJoystick->GetRawAxis(LOGITECH_Y_AXIS);

	speed = sqrt(x * x + y * y);

	return speed;
}

double LogitechJoystick::GetJoystickSpeedSquared(void)
{
	double speed = GetJoystickSpeed();

	return fabs(speed) * speed;
}

double LogitechJoystick::GetJoystickSpeedCubed(void)
{
	double speed = GetJoystickSpeed();
	
	return speed * speed * speed;
}

int LogitechJoystick::GetLogitechButton(int button)
{
	bool status = mJoystick->GetRawButton(button);
	
	if(status)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

#endif
