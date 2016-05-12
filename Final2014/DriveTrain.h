#ifndef DRIVE_TRAIN_H
#define DRIVE_TRAIN_H

class DriveTrain
{
public:
	DriveTrain(void);
	~DriveTrain(void);

	void TestP(double, double);
	void UpdateDrive(double, double);
	void AdjustDrive(double, double);
	void Drive(double, double);
	void StopDrive(void);
	void DriveTest(void);
	void SpinRobot(int);
	void ResetEncoder(void);
	void DriveStraight(void);
	void SetBackDrive(double);
	void SetFrontDrive(double);
	void SonarDrive(void);

	double GetBackDriveRaw(void);
	double GetFrontDriveRaw(void);
	double GetBackDriveDistance(void);
	double GetFrontDriveDistance(void);
	double GetBackDriveRate(void);
	double GetFrontDriveRate(void);
	double GetSonar(void);
	void GetEncoder(void);

	bool DriveForward(double);

private:
	Talon* mBackDrive;
	Talon* mFrontDrive;

	Encoder* mFrontDriveEncoder;
	Encoder* mBackDriveEncoder;

	Timer* mTimer;

	AnalogChannel* mSonar;

	double mFrontSpeed;
	double mBackSpeed;
	double mBackPConstant;
	double mFrontPConstant;
};

DriveTrain::DriveTrain(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mBackDrive = new Talon(MODULE_1, BACK_DRIVE_TALON);
	mFrontDrive = new Talon(MODULE_1, FRONT_DRIVE_TALON);

	mFrontDriveEncoder = new Encoder(MODULE_1, FRONT_DRIVE_ENCODER_A, MODULE_1, FRONT_DRIVE_ENCODER_B, false, Encoder::k2X);
	mBackDriveEncoder = new Encoder(MODULE_1, BACK_DRIVE_ENCODER_A, MODULE_1, BACK_DRIVE_ENCODER_B, false, Encoder::k2X);
	mFrontDriveEncoder->Reset();
	mBackDriveEncoder->Reset();
	mFrontDriveEncoder->SetDistancePerPulse(DRIVE_DISTANCE_PER_PULSE);
	mBackDriveEncoder->SetDistancePerPulse(DRIVE_DISTANCE_PER_PULSE);
	mFrontDriveEncoder->Start();
	mBackDriveEncoder->Start();

	mTimer = new Timer();
	mTimer->Reset();
	mTimer->Start();

	mFrontSpeed = .4;
	mBackSpeed = .4;
	mBackPConstant = 1;
	mFrontPConstant = 1;
}

DriveTrain::~DriveTrain(void)
{
	delete mBackDrive;
	mBackDrive = NULL;
	delete mFrontDrive;
	mFrontDrive = NULL;
	delete mFrontDriveEncoder;
	mFrontDriveEncoder = NULL;
	delete mBackDriveEncoder;
	mBackDriveEncoder = NULL;
	delete mTimer;
	mTimer = NULL;
}

void DriveTrain::TestP(double pot1, double pot2)
{
	double frontRate = mFrontDriveEncoder->GetRate();
	double backRate = mBackDriveEncoder->GetRate();

	printf("Pot 1: %.3f Pot 2: %.3f  Front Rate: %.3f Back Rate: %.3f  Ratio: %.3f\n", pot1, pot2, frontRate, backRate, frontRate / backRate);

	mFrontSpeed = pot1;
	mBackSpeed = mFrontSpeed * pot2;

	if(mBackSpeed > 1)
	{
		mBackSpeed = 1;
	}
	if(mFrontSpeed > 1)
	{
		mFrontSpeed = 1;
	}

	Drive(mFrontSpeed, mBackSpeed);
}

void DriveTrain::UpdateDrive(double frontSpeed, double backSpeed)
{
	Drive(frontSpeed, backSpeed);
}


bool  DriveTrain::DriveForward(double dist)
{
	bool stillDriving;
//	Drive(mFrontSpeed, mBackSpeed);
	SetFrontDrive(mFrontSpeed);
	SetBackDrive(mBackSpeed);
	double frontDistance = mFrontDriveEncoder->GetDistance();
	/*double frontRate = mFrontDriveEncoder->GetRate();
	double backRate = mBackDriveEncoder->GetRate();
	if(mFrontSpeed == 1 || mBackSpeed == 1)
	{
		if(frontRate < backRate)
		{
			mBackSpeed = mBackSpeed * (frontRate/backRate); //lower back speed
		}
		if(backRate < frontRate)
		{
			mFrontSpeed = mFrontSpeed * (backRate / frontRate); //lower front speed
		}
	}
	else
	{
		if(frontRate < backRate)
		{
			mFrontSpeed = mFrontSpeed * (backRate / frontRate); //raise front speed
		}
		if(backRate < frontRate)
		{
			mFrontSpeed = mFrontSpeed * (backRate / frontRate); //lower front speed
		}
	}

	if(mBackSpeed > 1)
		mBackSpeed = 1;
	if(mFrontSpeed > 1)
		mFrontSpeed = 1;*/

	if(frontDistance <= dist)
		stillDriving = true;
	else
	{
		stillDriving = false;
		//Stop();
	}
	printf("Front D: %f Back D: %f\n", mFrontDriveEncoder->GetDistance(), mBackDriveEncoder->GetDistance());
	return stillDriving;
}

void DriveTrain::AdjustDrive(double frontSpeed, double backSpeed)
{
	double backDistance, frontDistance;
	backDistance = fabs(mBackDriveEncoder->GetDistance());
	frontDistance = fabs(mFrontDriveEncoder->GetDistance());

	if(backDistance > 0 && frontDistance > 0)
	{
		if((backDistance < frontDistance && fabs(backSpeed) < 1) || (backDistance > frontDistance && fabs(frontSpeed) == 1))
		{
			mBackPConstant = frontDistance / backDistance;
			mBackSpeed = backSpeed * mBackPConstant;
			mFrontPConstant = 1;
			mFrontSpeed = frontSpeed;
			if(fabs(mBackSpeed) > 1)
			{
				mBackSpeed = backSpeed;
				mBackPConstant = 1;
				mFrontPConstant = backDistance / frontDistance;
				mFrontSpeed = frontSpeed * mFrontPConstant;
			}
		}
		else if((backDistance < frontDistance && fabs(backSpeed) == 1) || (backDistance > frontDistance && fabs(frontSpeed) < 1))
		{
			mFrontPConstant = backDistance / frontDistance;
			mFrontSpeed = frontSpeed * mFrontPConstant;
			mBackPConstant = 1;
			mBackSpeed = backSpeed;
			if(fabs(mBackSpeed) > 1)
			{
				mFrontSpeed = frontSpeed;
				mFrontPConstant = 1;
				mBackPConstant = frontDistance / backDistance;
				mBackSpeed = backSpeed * mBackPConstant;
			}
		}
	}
	else
	{
		mFrontSpeed = frontSpeed;
		mBackSpeed = backSpeed;
	}

	printf("LD Encoder: %f  RD Encoder: %f  ", mFrontDriveEncoder->GetRate(), mBackDriveEncoder->GetRate());
	printf("FrontSpeed = %.3f  BackSpeed = %.3f  FrontConstant = %.3f  BackConstant = %.3f\n", mFrontSpeed, mBackSpeed, mFrontPConstant, mBackPConstant);

}

void DriveTrain::Drive(double frontSpeed, double backSpeed)
{
	if(mTimer->Get() >= .1)
	{
		AdjustDrive(frontSpeed, backSpeed);
		mTimer->Reset();
	}
	if(mBackSpeed != 0.0)
		mBackDrive->Set(mBackSpeed);
	else
		mBackDrive->Disable();
	if(mFrontSpeed != 0.0)
		mFrontDrive->Set(mFrontSpeed);
	else
		mFrontDrive->Disable();
}

void DriveTrain::StopDrive(void)
{
	mBackDrive->Disable();
	mFrontDrive->Disable();
}

void DriveTrain::DriveTest(void)
{
	printf("frontSet, backSet = 0.6\n");
	Drive(0.6, 0.6);
	printf("frontSpeed = %f  backSpeed = %f", mFrontDrive->Get(), mBackDrive->Get());
	Wait(2);
	printf("Stop motors\n");
	StopDrive();
	printf("frontSpeed = %f  backSpeed = %f", mFrontDrive->Get(), mBackDrive->Get());
	Wait(2);
	printf("frontSpeed, backSpeed = -0.6\n");
	Drive(-0.6, -0.6);
	printf("frontSpeed = %f  backSpeed = %f", mFrontDrive->Get(), mBackDrive->Get());
	Wait(2);
	printf("Stop motors\n");
	StopDrive();
	printf("frontSpeed = %f  backSpeed = %f", mFrontDrive->Get(), mBackDrive->Get());
	Wait(2);
}

void DriveTrain::SpinRobot(int direction)
{
	mBackDrive->Set(.6 * direction);
	mFrontDrive->Set(-.6 * direction);
}

void DriveTrain::ResetEncoder(void)
{
	mFrontDriveEncoder->Reset();
	mBackDriveEncoder->Reset();
	mFrontDriveEncoder->Start();
	mBackDriveEncoder->Start();
}

void DriveTrain::DriveStraight(void)
{
	double frontRate = mFrontDriveEncoder->GetRate();
	double backRate = mBackDriveEncoder->GetRate();
	if(mFrontSpeed == 1 || mBackSpeed == 1)
	{
		if(frontRate < backRate)
		{
			mBackSpeed = mBackSpeed * (frontRate/backRate); //lower Back speed
		}
		if(backRate < frontRate)
		{
			mFrontSpeed = mFrontSpeed * (backRate / frontRate); //lower front speed
		}
	}
	else
	{
		if(frontRate < backRate)
		{
			mFrontSpeed = mFrontSpeed * (backRate / frontRate); //raise front speed
		}
		if(backRate < frontRate)
		{
			mFrontSpeed = mFrontSpeed * (backRate / frontRate); //lower front speed
		}
	}

	if(mBackSpeed > 1)
	{
		mBackSpeed = 1;
	}
	if(mFrontSpeed > 1)
	{
		mFrontSpeed = 1;
	}
}

double DriveTrain::GetBackDriveRaw(void)
{
	return mBackDriveEncoder->Get();
}

double DriveTrain::GetFrontDriveRaw(void)
{
	return mFrontDriveEncoder->Get();
}
double DriveTrain::GetBackDriveDistance(void)
{
	return mBackDriveEncoder->GetDistance();
}

double DriveTrain::GetFrontDriveDistance(void)
{
	return mFrontDriveEncoder->GetDistance();
}

double DriveTrain::GetBackDriveRate(void)
{
	return mBackDriveEncoder->GetRate();
}

double DriveTrain::GetFrontDriveRate(void)
{
	return mFrontDriveEncoder->GetRate();
}

void DriveTrain::SetBackDrive(double power)
{
	mBackDrive->Set(power);
}

void DriveTrain::SetFrontDrive(double power)
{
	mFrontDrive->Set(power);
}

double DriveTrain::GetSonar(void)
{
	return (mSonar->GetAverageValue() + 5) / 1.9;
}

void DriveTrain::GetEncoder(void)
{
	printf("FEGet: %ld  FEGetRaw: %ld  BEGet: %ld  BEGetRaw: %ld\n", mFrontDriveEncoder->Get(), mFrontDriveEncoder->GetRaw(), mBackDriveEncoder->Get(), mBackDriveEncoder->GetRaw());
}

void DriveTrain::SonarDrive(void)
{
	int direction;
	double distance = GetSonar();
	
	if(distance - SONAR_SHOOTING_DISTANCE > SONAR_TOLERANCE)
	{
		direction = 1;
	}
	else if(SONAR_SHOOTING_DISTANCE - distance < SONAR_TOLERANCE)
	{
		direction = -1;
	}
	else
	{
		direction = 0;
	}
	
	UpdateDrive(direction * .3, direction * .3);
}

#endif
