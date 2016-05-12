#ifndef DRIVE_H
#define DRIVE_H

class Drive
{
public:
	Drive(Talon*, Talon*, RobotDrive*, LHSJoystick*, AnalogInput*, AnalogInput*, Encoder*, Encoder*, AnalogInput*);
	~Drive();
	bool TestDrive();
	bool DriveUntilDistance(int);
	bool DriveUntilClose();
	bool DriveDistWithEncoder(double, int);
	bool TurnDistWithEncoder(double, int);
	bool TurnAngleWithEncoder(double, int);
	void ResetEncoders();
	double GetIRDistance();

private:
	Talon* mRightMotor;
	Talon* mLeftMotor;
	RobotDrive* mRobot;
	LHSJoystick* mXbox;
	AnalogInput* mSonar;
	AnalogInput* mCalibrateSonar;
	Encoder* mLeftEncoder;
	Encoder* mRightEncoder;
	AnalogInput* mIRSensor;
	int counter;
};

Drive::Drive(Talon* pRightMotor, Talon* pLeftMotor, RobotDrive* pRobot, LHSJoystick* pXbox, AnalogInput* pSonar,
		AnalogInput* pCalibrateSonar, Encoder* pLeftEncoder, Encoder* pRightEncoder, AnalogInput* pIRSensor)
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
	mRightMotor = pRightMotor;
	mLeftMotor = pLeftMotor;
	mRobot = pRobot;
	mXbox = pXbox;
	mSonar = pSonar;
	mCalibrateSonar = pCalibrateSonar;
	mLeftEncoder = pLeftEncoder;
	mRightEncoder = pRightEncoder;
	mIRSensor = pIRSensor;
	mLeftEncoder->SetDistancePerPulse(DRIVE_DISTANCE_PER_PULSE);
	mRightEncoder->SetDistancePerPulse(DRIVE_DISTANCE_PER_PULSE);
	mLeftEncoder->Reset();
	mRightEncoder->Reset();
	counter = 0;
}

Drive::~Drive()
{
	delete mIRSensor;
	mIRSensor = NULL;
	delete mRightEncoder;
	mRightEncoder = NULL;
	delete mLeftEncoder;
	mLeftEncoder = NULL;

	delete mCalibrateSonar;
	mCalibrateSonar = NULL;
	delete mSonar;
	mSonar = NULL;

	delete mXbox;
	mXbox = NULL;

	delete mRobot;
	mRobot = NULL;
	delete mLeftMotor;
	mLeftMotor = NULL;
	delete mRightMotor;
	mRightMotor = NULL;
}

void Drive::ResetEncoders()
{
	mLeftEncoder->Reset();
	mRightEncoder->Reset();
}

bool Drive::TurnDistWithEncoder(double dist, int direction)
{
	bool doneTurning = false;
	double leftDistance = mLeftEncoder->GetDistance();
	double rightDistance = mRightEncoder->GetDistance();

	if(fabs(leftDistance) >= dist || fabs(rightDistance) >= dist)
	{
		mRobot->TankDrive(0.0,0.0,true);
		doneTurning = true;
	}
	else
	{
		mRobot->TankDrive(0.65*(57.0/60.0)*direction,-0.65*direction,true);
	}
	//printf("Left  Encoder Ticks: %d, Left  Encoder Dist: %f\n", mLeftEncoder->GetRaw(), mLeftEncoder->GetDistance());
	//printf("Right Encoder Ticks: %d, Right Encoder Dist: %f\n", mRightEncoder->GetRaw(), mRightEncoder->GetDistance());
	return doneTurning;
}

bool Drive::TurnAngleWithEncoder(double angle, int direction)
{
	bool doneTurning = false;
	double leftDistance = mLeftEncoder->GetDistance();
	double rightDistance = mRightEncoder->GetDistance();

	if((fabs(leftDistance)*360.0)/74.0 >= angle || (fabs(rightDistance)*360.0)/74.0 >= angle)
	{
		mRobot->TankDrive(0.0,0.0,true);
		doneTurning = true;
	}
	else
	{
		mRobot->TankDrive(0.65*(57.0/60.0)*direction,-0.65*direction,true);
	}
	//printf("Left  Encoder Ticks: %d, Left  Encoder Dist: %f\n", mLeftEncoder->GetRaw(), mLeftEncoder->GetDistance());
	//printf("Right Encoder Ticks: %d, Right Encoder Dist: %f\n", mRightEncoder->GetRaw(), mRightEncoder->GetDistance());
	return doneTurning;
}

bool Drive::DriveDistWithEncoder(double dist, int direction)
{
	bool doneDriving = false;
	double leftDistance = mLeftEncoder->GetDistance();
	double rightDistance = mRightEncoder->GetDistance();

	if(fabs(leftDistance) >= dist || fabs(rightDistance) >= dist)
	{
		mRobot->TankDrive(0.0,0.0,true);
		doneDriving = true;
	}
	else
	{
		mRobot->TankDrive(-0.60*(57.0/60.0)*direction,-0.60*direction,true);
	}

	printf("Left  Encoder Ticks: %d, Left  Encoder Dist: %f\n", mLeftEncoder->GetRaw(), mLeftEncoder->GetDistance());
	printf("Right Encoder Ticks: %d, Right Encoder Dist: %f\n", mRightEncoder->GetRaw(), mRightEncoder->GetDistance());
	return doneDriving;
}

bool Drive::DriveUntilDistance(int dist)
{
	double sonarVolts = mSonar->GetAverageVoltage();
	double calibrateVolts = mCalibrateSonar->GetAverageVoltage();
	double distance = (sonarVolts / ((calibrateVolts <= 0 ? 4.85 : calibrateVolts) / 512));
	bool isDoneDriving = false;

	if(distance <= dist)
	{
		mRobot->ArcadeDrive(0.0,0.0,true);
		isDoneDriving = true;
	}
	else if(distance >= dist + 2)
	{
		mRobot->ArcadeDrive(.5, 0.0, true);
		isDoneDriving = false;
	}
	else
	{
		mRobot->ArcadeDrive(.5 * ((distance - dist)/2), 0.0, true);
		isDoneDriving = false;
	}
	return isDoneDriving;
}

bool Drive::DriveUntilClose()
{
	double IRValue = GetIRDistance();
	bool isDoneApproaching = false;

	if(IRValue > 7.0)
	{
		mRobot->ArcadeDrive(0.5, 0.0, true);
		isDoneApproaching = false;
	}
	else
	{
		mRobot->ArcadeDrive(0.0,0.0,true);
		isDoneApproaching = true;
	}
	return isDoneApproaching;
}

bool Drive::TestDrive() //by Jaxson
{
	if(mXbox->GetRawButton(XBOX_X_BUTTON) == true)
		return true;

	if(mXbox->GetRawButton(XBOX_A_BUTTON) == true)
	{
		while(mXbox->GetRawButton(XBOX_A_BUTTON) == true)
		{}
		counter++;

		if(counter > 2)
			counter = 0;
	}

	if(counter == 0)
	{
		//printf("Testing Left Motor\n");

		//mRobot->TankDrive(mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS), 0.0, true);
	}
	if(counter == 1)
	{
		//printf("Testing Right Motor\n");

		//mRobot->TankDrive(0.0, mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS), true);
	}
	if(counter == 2)
	{
		//printf("Testing Both Motors\n");

		//mRobot->ArcadeDrive(mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS), -mXbox->GetRawAxis(XBOX_LEFT_X_AXIS));
	}
	return false;
}

double Drive::GetIRDistance()
{
	double IRValue = mIRSensor->GetVoltage();
	printf("IR Distance:  %f\n", 10.5151 * pow(IRValue, -1.14676));
	return  10.5151 * pow(IRValue, -1.14676);
}

#endif
