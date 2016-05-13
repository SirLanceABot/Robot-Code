#ifndef XBOXJOYSTICK_H
#define XBOXJOYSTICK_H

class XBoxJoystick
{
public:
	enum Button{kNone = 0, kA = 1, kB = 2, kX = 3, kY = 4, kLBumper=5, kRBumper=6, kBack = 7, kStart = 8, kLStick = 9, kRStick = 10};
	enum Axis{kLeftXAxis = 1, kLeftYAxis = 2, kLeftZAxis = 3, kRightZAxis = 4, kRightXAxis = 5, kRightYAxis = 6};
	XBoxJoystick(void);
	~XBoxJoystick(void);
	void JoystickTest(void);
	void WaitForX(void);
	double GetLeftJoystickAngle(void);
	double GetRightJoystickAngle(void);
	double GetLeftJoystickHypotenuse(void);
	double GetRightJoystickHypotenuse(void);
	bool GetXBoxButton(XBoxJoystick::Button);
	double GetAxis(Axis);
	void WaitForButton(Button);
	void Print(CrabDriveMain * const mCrabDriveMain);
private:
	Joystick* mJoystick;
};

XBoxJoystick::XBoxJoystick(void)
{
	printf("File %s Date %s Time %s\n",__FILE__,__DATE__, __TIME__);
	mJoystick = new Joystick(XBOX_CONTROLLER);
}

XBoxJoystick::~XBoxJoystick(void)
{
	delete mJoystick;
	mJoystick = NULL;
}

void XBoxJoystick::JoystickTest(void)
{
	double x, y, angle;
	while(mJoystick->GetRawButton(kBack)!= 1)
	{
		x = mJoystick->GetRawAxis(LEFT_X_AXIS);
		y = mJoystick->GetRawAxis(LEFT_Y_AXIS);

		angle = GetLeftJoystickAngle();

		printf("X = %f  Y = %f  Angle = %f\n", x, y,  angle);

		Wait(0.1);
	}
}

void XBoxJoystick::WaitForX(void)
{
	while(mJoystick->GetRawButton(kX) != 1)
	{
		Wait(.001);	
	}	
	while(mJoystick->GetRawButton(kX) == 1)
	{
		Wait(.001);
	}
}

double XBoxJoystick::GetLeftJoystickAngle(void)
{
	//0 degrees is forward, ccW 0 to 360
	double x, y, angle;

	x = mJoystick->GetRawAxis(LEFT_X_AXIS);
	y = -mJoystick->GetRawAxis(LEFT_Y_AXIS);

	angle = (atan2(y,x) * (180.0 / PI)) - 90.;

	if(angle < 0.0)
		angle = angle + 360.0;

	return(angle);
}

double XBoxJoystick::GetRightJoystickAngle(void)
{
	double x, y, angle;

	x = mJoystick->GetRawAxis(RIGHT_X_AXIS);
	y = -mJoystick->GetRawAxis(RIGHT_Y_AXIS);

	angle = (atan2(y,x) * (180.0 / PI)) - 90.;

	if(angle < 0.0)
		angle = angle + 360.0;

	return(angle);
}

double XBoxJoystick::GetLeftJoystickHypotenuse(void)
{
	double x, y, speed;

	x = mJoystick->GetRawAxis(LEFT_X_AXIS);
	y = mJoystick->GetRawAxis(LEFT_Y_AXIS);

	speed = sqrt(x * x + y * y);

	if (speed >  1.) speed =  1.;

	speed = speed*speed; // make stick less responsive down low

	return speed;
}

double XBoxJoystick::GetRightJoystickHypotenuse(void)
{
	double x, y, speed;

	x = mJoystick->GetRawAxis(RIGHT_X_AXIS);
	y = mJoystick->GetRawAxis(RIGHT_Y_AXIS);

	speed = sqrt(x * x + y * y);

	if (speed >  1.) speed =  1.;

	speed = speed*speed; // make stick less responsive down low

	return speed;
}

bool XBoxJoystick::GetXBoxButton(XBoxJoystick::Button button)
{
		return mJoystick->GetRawButton(button);
}

double XBoxJoystick::GetAxis(Axis currentAxis)
{
		return mJoystick->GetRawAxis(currentAxis);
}

void XBoxJoystick::WaitForButton(Button currentPress)
{
	while(mJoystick->GetRawButton(currentPress) != 1)
	{
		Wait(.001);	
	}	
	while(mJoystick->GetRawButton(currentPress) == 1)
	{
		Wait(.001);
	}
}

void XBoxJoystick::Print(CrabDriveMain * const mCrabDriveMain)
{
//printf("JLX:%f JLY:%f ", mJoystick->GetRawAxis(LEFT_X_AXIS), mJoystick->GetRawAxis(LEFT_Y_AXIS));
printf("%f %f %f %f %f %f %d %d %d %d %d %d %d %d %d %d\n",
		mJoystick->GetRawAxis(1), mJoystick->GetRawAxis(2),mJoystick->GetRawAxis(3),
		mJoystick->GetRawAxis(4),mJoystick->GetRawAxis(5), mJoystick->GetRawAxis(6),
		mJoystick->GetRawButton(1), mJoystick->GetRawButton(2), mJoystick->GetRawButton(3), mJoystick->GetRawButton(4), mJoystick->GetRawButton(5),
		mJoystick->GetRawButton(6), mJoystick->GetRawButton(7), mJoystick->GetRawButton(8), mJoystick->GetRawButton(9), mJoystick->GetRawButton(10));
}

#endif
