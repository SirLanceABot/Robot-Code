#ifndef DRIVE_TRAIN_H
#define DRIVE_TRAIN_H

class DriveTrain
{
public:
	DriveTrain(void);
	~DriveTrain(void);
	void UpdateDrive(double, double);
	bool DriveForward(double);
	void Drive(double, double);
	void Stop(void);
	void ResetEncoder(void);
	void DriveStraight(void);
	void SpinRobot(int);
	void Print(void);

private:
	Talon* mRightDrive;
	Talon* mLeftDrive;
	Encoder* mLeftDriveEncoder;
	Encoder* mRightDriveEncoder;
	double mLeftSpeed, mRightSpeed;
};

DriveTrain::DriveTrain(void)
{
	printf("File %s Date %s Time %s\n",__FILE__,__DATE__, __TIME__);
	mRightDrive = new Talon(MODULE_1, RIGHT_DRIVE_TALON);
	mLeftDrive = new Talon(MODULE_1, LEFT_DRIVE_TALON);
	mLeftDriveEncoder = new Encoder(MODULE_1, LEFT_DRIVE_ENCODER_A, MODULE_1, LEFT_DRIVE_ENCODER_B, false, Encoder::k1X);
	mRightDriveEncoder = new Encoder(MODULE_1, RIGHT_DRIVE_ENCODER_A, MODULE_1, RIGHT_DRIVE_ENCODER_B, false, Encoder::k1X);
	mLeftDriveEncoder->Reset();
	mRightDriveEncoder->Reset();
	mLeftDriveEncoder->SetDistancePerPulse((WHEEL_DIAMETER/12.0*PI)/1000.0);
	mRightDriveEncoder->SetDistancePerPulse((WHEEL_DIAMETER/12.0*PI)/1000.0);
	mLeftDriveEncoder->Start();
	mRightDriveEncoder->Start();
	mLeftSpeed = .6;
	mRightSpeed = .6;
}

DriveTrain::~DriveTrain(void)
{
	delete mRightDrive;
	mRightDrive = NULL;
	delete mLeftDrive;
	mLeftDrive = NULL;
	delete mLeftDriveEncoder;
	mLeftDriveEncoder = NULL;
	delete mRightDriveEncoder;
	mRightDriveEncoder = NULL;
}

void DriveTrain::UpdateDrive(double leftSpeed, double rightSpeed)
{
	printf(" DriveTrain::UpdateDrive LD Encoder: %f  RD Encoder: %f\n", mLeftDriveEncoder->GetRate(), mRightDriveEncoder->GetRate());
	Drive(leftSpeed, rightSpeed);
}

bool  DriveTrain::DriveForward(double dist)
{
	bool stillDriving;
	Drive(mLeftSpeed,mRightSpeed);
	double leftRate = mLeftDriveEncoder->GetRate();
	double rightRate = mRightDriveEncoder->GetRate();
	if(mLeftSpeed == 1 || mRightSpeed == 1)
	{
		if(leftRate < rightRate)
		{
			mRightSpeed = mRightSpeed * (leftRate/rightRate); //lower right speed
		}
		if(rightRate < leftRate)
		{
			mLeftSpeed = mLeftSpeed * (rightRate / leftRate); //lower left speed
		}
	}
	else
	{
		if(leftRate < rightRate)
		{
			mLeftSpeed = mLeftSpeed * (rightRate / leftRate); //raise left speed
		}
		if(rightRate < leftRate)
		{
			mLeftSpeed = mLeftSpeed * (rightRate / leftRate); //lower left speed
		}
	}

	if(mRightSpeed > 1)
		mRightSpeed = 1;
	if(mLeftSpeed > 1)
		mLeftSpeed = 1;

	if(leftRate <= dist)
		stillDriving = true;
	else
		stillDriving = false;

	printf("Left D: %f RightD: %f\n", mLeftDriveEncoder->GetDistance(), mRightDriveEncoder->GetDistance());
	return stillDriving;
}

void DriveTrain::Drive(double leftSpeed, double rightSpeed)
{
	//printf(" DriveTrain::Drive LS %f, RS %f, ", leftSpeed, rightSpeed);

	if(leftSpeed != 0.0) mLeftDrive->Set(leftSpeed);
	else mLeftDrive->Disable();

	if(rightSpeed != 0.0) mRightDrive->Set(rightSpeed);
	else mRightDrive->Disable();
}

void DriveTrain::Stop(void)
{
	mRightDrive->Disable();
	mLeftDrive->Disable();
}

void DriveTrain::ResetEncoder(void)
{
	mLeftDriveEncoder->Reset();
	mRightDriveEncoder->Reset();
	mLeftDriveEncoder->Start();
	mRightDriveEncoder->Start();
}

void DriveTrain::DriveStraight(void)
{
	double leftRate = mLeftDriveEncoder->GetRate();
	double rightRate = mRightDriveEncoder->GetRate();
	if(mLeftSpeed == 1 || mRightSpeed == 1)
	{
		if(leftRate < rightRate)
		{
			mRightSpeed = mRightSpeed * (leftRate/rightRate); //lower right speed
		}
		if(rightRate < leftRate)
		{
			mLeftSpeed = mLeftSpeed * (rightRate / leftRate); //lower left speed
		}
	}
	else
	{
		if(leftRate < rightRate)
		{
			mLeftSpeed = mLeftSpeed * (rightRate / leftRate); //raise left speed
		}
		if(rightRate < leftRate)
		{
			mLeftSpeed = mLeftSpeed * (rightRate / leftRate); //lower left speed
		}
	}

	if(mRightSpeed > 1)
	{
		mRightSpeed = 1;
	}
	if(mLeftSpeed > 1)
	{
		mLeftSpeed = 1;
	}
}

void DriveTrain::SpinRobot(int direction)
{
	mLeftDrive->Set(.6 * direction);
	mRightDrive->Set(-.6 * direction);
}

void DriveTrain::Print(void)
{
printf("ELDist:%f ERDist:%f ELRaw:%ld ERRaw:%ld ", mLeftDriveEncoder->GetDistance(), mRightDriveEncoder->GetDistance(), mLeftDriveEncoder->GetRaw(), mRightDriveEncoder->GetRaw());
printf("ELRate:%f ERRate:%f ", mLeftDriveEncoder->GetRate(), mRightDriveEncoder->GetRate());
printf("LSpeed:%f RSpeed:%f ", mLeftDrive->Get(), mRightDrive->Get());
}


#endif
