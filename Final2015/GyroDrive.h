#ifndef GYRO_DRIVE_H
#define GYRO_DRIVE_H

enum SteerDirection{kCCW = 1, kCW = -1, kStop = 0};
enum DriveDirection{kForward = 1, kReverse = -1, kOff = 0};

class GyroDrive
{
public:
	GyroDrive(Gyro*, Talon*, Talon*, LHSJoystick*, RobotDrive*, DigitalInput*, DigitalInput*, DigitalInput*, DigitalInput*);
	~GyroDrive(void);

	void ResetGyro(void);
	double NormalizeAngle(double);
	float TestAngle(void);
	void TurnAngle(float);
	bool TurnAngle(float,int,int);
	void DriveAfterTurn(DriveDirection);
	int GetDial();
	float GetGyro();

private:
	Gyro* mGyro;
	Talon* mLeftMotor;
	Talon* mRightMotor;
	LHSJoystick* mXbox;
	RobotDrive* mRobot;
	DigitalInput* mDialOnes;
	DigitalInput* mDialTwos;
	DigitalInput* mDialFours;
	DigitalInput* mDialEights;
};

GyroDrive::GyroDrive(Gyro* pGyro, Talon* pLeftMotor, Talon* pRightMotor, LHSJoystick* pXbox, RobotDrive* pRobot,
		DigitalInput* pDialOnes, DigitalInput* pDialTwos, DigitalInput* pDialFours, DigitalInput* pDialEights)
{
	for(int x = 0; x < 20; x++)
		printf("\n");
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
	mGyro = pGyro;
	mGyro->Reset();
	mLeftMotor = pLeftMotor;
	mRightMotor = pRightMotor;
	mXbox = pXbox;
	mRobot = pRobot;
	mDialOnes = pDialOnes;
	mDialTwos = pDialTwos;
	mDialFours = pDialFours;
	mDialEights = pDialEights;
}

GyroDrive::~GyroDrive(void)
{
	delete mDialEights;
	mDialEights = NULL;
	delete mDialFours;
	mDialFours = NULL;
	delete mDialTwos;
	mDialTwos = NULL;
	delete mDialOnes;
	mDialOnes = NULL;
	delete mRobot;
	mRobot = NULL;
	delete mXbox;
	mXbox = NULL;
	delete mRightMotor;
	mRightMotor = NULL;
	delete mLeftMotor;
	mLeftMotor = NULL;
	delete mGyro;
	mGyro = NULL;
}

void GyroDrive::ResetGyro(void)
{
	if(mXbox->GetRawButton(XBOX_Y_BUTTON) == true)
		mGyro->Reset();
}

double GyroDrive::NormalizeAngle(double angle)
{
	angle = angle - int(angle / 360.0) * 360.0;

	if(angle < 0)
	{
		angle = angle + 360.0;
	}

	return angle;
}

float GyroDrive::TestAngle(void)
{
	float X_Axis, Y_Axis;
	double joystickAngle;
	X_Axis = mXbox->GetRawAxis(XBOX_LEFT_X_AXIS, 0);
	Y_Axis = mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS, 0);

	joystickAngle = atan2 (Y_Axis, X_Axis) * 180.0 / PI;
	if (joystickAngle < 0)
	{
		joystickAngle = joystickAngle + 360;
	}

	return joystickAngle;
}

void GyroDrive::DriveAfterTurn(DriveDirection dir)
{
	//float magnitude = sqrt(pow(mXbox->GetRawAxis(XBOX_LEFT_X_AXIS),2) + pow(mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS),2));
	float magnitude = mXbox->GetMagnitude();
	mRobot->ArcadeDrive(magnitude * dir, 0.0, true);
}

int GyroDrive::GetDial()
{
	int dial = 0;
	if(mDialOnes->Get())
	{
		dial = dial + 1;
	}
	if(mDialTwos->Get())
	{
		dial = dial + 2;
	}
	if(mDialFours->Get())
	{
		dial = dial + 4;
	}
	if(mDialEights->Get())
	{
		dial = dial + 8;
	}
	dial = (19 - dial) % 16;
	printf("Dial:  %d", dial);
	return dial;
}
float GyroDrive::GetGyro()
{
	return mGyro->GetAngle() + GetDial() * 22.5;
}

void GyroDrive::TurnAngle(float joystickAngle) // Change control scheme for override turn (stationary turn)
{
	float gyroAngle;
	SteerDirection turnDirection; //enum kCCW = 1 or kCW = -1 or kStop = 0
	DriveDirection driveDirection; //enum kForward = 1 or kReverse = -1 or kOff = 0

	gyroAngle = mGyro->GetAngle();

	float turnAngle = NormalizeAngle(joystickAngle - gyroAngle);

	if(mXbox->GetRawButton(XBOX_X_BUTTON) == true) // x button to force front of robot
	{
		if(turnAngle < 180)
		{
			turnDirection = kCW;
			driveDirection = kForward;
		}
		else
		{
			turnDirection = kCCW;
			driveDirection = kForward;
		}
	}
	else
	{
		if(turnAngle < 90)
		{
			turnDirection = kCW;
			driveDirection = kForward;
			//printf(", TA < 90, ");
		}
		else if(turnAngle < 180)
		{
			turnDirection = kCCW;
			driveDirection = kReverse;
			turnAngle = NormalizeAngle(180 - turnAngle);
			//printf(", TA < 180, ");
		}
		else if(turnAngle < 270)
		{
			turnDirection = kCW;
			driveDirection = kReverse;
			turnAngle = NormalizeAngle(turnAngle + 180.0);
			//printf(", TA < 270, ");
		}
		else if(turnAngle < 360)
		{
			turnDirection = kCCW;
			driveDirection = kForward;
			turnAngle = NormalizeAngle(360 - turnAngle);
			//printf(", TA < 360, ");
		}
	}

	if(turnAngle > MAX_ANGLE_TOLERANCE)
	{
		if(turnDirection == kCCW)
		{
			mRobot->ArcadeDrive(0.0,.6*turnDirection,true);
		}
		else
		{
			mRobot->ArcadeDrive(0.0,.6*turnDirection,true);
		}
	}
	else if(turnAngle > MIN_ANGLE_TOLERANCE)
	{
		if(turnDirection == kCCW)
		{
			mRobot->ArcadeDrive(0.0,.45*driveDirection, true);
		}
		else
		{
			mRobot->ArcadeDrive(0.0,.45*driveDirection, true);
		}
	}
	else if(mXbox->GetRawButton(XBOX_B_BUTTON) == 0)
	{
		DriveAfterTurn(driveDirection);
	}
}

bool GyroDrive::TurnAngle(float joystickAngle, int forceTurn, int driveAfter) // Change control scheme for override turn (stationary turn)
{
	float gyroAngle;
	bool isDoneTurning = false;
	SteerDirection turnDirection; //enum kCCW = 1 or kCW = -1 or kStop = 0
	DriveDirection driveDirection; //enum kForward = 1 or kReverse = -1 or kOff = 0

	gyroAngle = mGyro->GetAngle();
	printf("Gyro: %f\n", gyroAngle);

	float turnAngle = NormalizeAngle(joystickAngle - gyroAngle);

	if(forceTurn == 1)
	{
		if(turnAngle < 180)
		{
			turnDirection = kCW;
			driveDirection = kForward;
		}
		else
		{
			turnDirection = kCCW;
			driveDirection = kForward;
		}
	}
	else
	{
		if(turnAngle < 90)
		{
			turnDirection = kCW;
			driveDirection = kForward;
			//printf(", TA < 90, ");
		}
		else if(turnAngle < 180)
		{
			turnDirection = kCCW;
			driveDirection = kReverse;
			turnAngle = NormalizeAngle(180 - turnAngle);
			//printf(", TA < 180, ");
		}
		else if(turnAngle < 270)
		{
			turnDirection = kCW;
			driveDirection = kReverse;
			turnAngle = NormalizeAngle(turnAngle + 180.0);
			//printf(", TA < 270, ");
		}
		else if(turnAngle < 360)
		{
			turnDirection = kCCW;
			driveDirection = kForward;
			turnAngle = NormalizeAngle(360 - turnAngle);
			//printf(", TA < 360, ");
		}
	}

	if(turnAngle > MAX_ANGLE_TOLERANCE)
	{
		if(turnDirection == kCCW)
		{
			mRobot->ArcadeDrive(0.0,.55*turnDirection,true);
		}
		else
		{
			mRobot->ArcadeDrive(0.0,.55*turnDirection,true);
		}
	}
	else if(turnAngle > MIN_ANGLE_TOLERANCE)
	{
		if(turnDirection == kCCW)
		{
			mRobot->ArcadeDrive(0.0,.4*driveDirection, true);
		}
		else
		{
			mRobot->ArcadeDrive(0.0,.4*driveDirection, true);
		}
	}
	else
	{
		isDoneTurning = true;
	}

	return isDoneTurning;
}

#endif
