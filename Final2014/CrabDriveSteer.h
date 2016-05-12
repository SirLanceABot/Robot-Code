#ifndef CRAB_DRIVE_STEER_H
#define CRAB_DRIVE_STEER_H

class CrabDriveSteer
{
public:
	enum SteerDirection{kCCW = 1, kCW = -1, kStop = 0};
	enum DriveDirection{kForward = 1, kReverse = -1, kOff = 0};

	CrabDriveSteer(void);
	~CrabDriveSteer(void);

	void UpdateSteer(double);
	void SetFrontWheels(double);
	void SetBackWheels(double);
	void SetTargetHeading(void);
	void StopAllWheels(void);
	void SteerFrontWheels(double);
	void SteerBackWheels(double);
	void SteerAllWheels(double);
	void AdjustSteer(void);
	void ResetWheels(void);
	void UpdateSmartDashboard(double);
	void ResetEncoder(void);
	void ResetGyro(void);

	void GetDriveDirection(double);

	bool TightTurn(double);
	bool AdjustHeading(void);

	double GetFrontHeading(void);
	double GetBackHeading(void);
	double GetRobotHeading(void);

	double NormalizeAngle(double);

	DriveDirection GetRightDirection(void);
	DriveDirection GetLeftDirection(void);
	SteerDirection GetSteerDirection(double);

private:
	Talon* mFrontSteer;
	Talon* mBackSteer;

	Encoder* mFrontEncoder;
	Encoder* mBackEncoder;
	Counter* mFrontCounter;
	Counter* mBackCounter;

	Gyro* mGyro;

	DriveDirection mLeftDirection;
	DriveDirection mRightDirection;

	double mFrontPConstant;
	double mBackPConstant;
	double mTargetHeading;
	double mSpinDirection;

	bool mIsFrontTurning;
	bool mIsBackTurning;
}; 

CrabDriveSteer::CrabDriveSteer(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
	mFrontSteer = new Talon(MODULE_1, FRONT_STEER_TALON);
	mBackSteer = new Talon(MODULE_1, BACK_STEER_TALON);

	mFrontEncoder = new Encoder(MODULE_1, FRONT_ENCODER_A, MODULE_1, FRONT_ENCODER_B, false, Encoder::k4X);
	mBackEncoder = new Encoder(MODULE_1, BACK_ENCODER_A, MODULE_1, BACK_ENCODER_B, false, Encoder::k4X);
	mFrontEncoder->Reset();
	mBackEncoder->Reset();
	mFrontEncoder->Start();
	mBackEncoder->Start();

	mFrontCounter = new Counter(MODULE_1, FRONT_ENCODER_INDEX);
	mBackCounter = new Counter(MODULE_1, BACK_ENCODER_INDEX);
	mFrontCounter->Reset();
	mBackCounter->Reset();
	mFrontCounter->Start();
	mBackCounter->Start();

	mGyro = new Gyro(GYRO_ANALOG_PORT);
	mGyro->Reset();

	mLeftDirection = kForward;
	mRightDirection = kForward;

	mIsFrontTurning = false;
	mIsBackTurning = false;

	mFrontPConstant = 1;
	mBackPConstant = 1;
	mTargetHeading = GetRobotHeading();
}

CrabDriveSteer::~CrabDriveSteer(void)
{
	delete mFrontSteer;
	mFrontSteer = NULL;
	delete mBackSteer;
	mBackSteer = NULL;
	delete mFrontEncoder;
	mFrontEncoder = NULL;
	delete mBackEncoder;
	mBackEncoder = NULL;
	delete mFrontCounter;
	mFrontCounter = NULL;
	delete mBackCounter;
	mBackCounter = NULL;
	delete mGyro;
	mGyro = NULL;
}

void CrabDriveSteer::UpdateSteer(double angle)
{
/*	if(AdjustHeading())
	{
		TightTurn(5 * mSpinDirection);
	}*/
	AdjustSteer();
	SteerAllWheels(angle);
	
}

void CrabDriveSteer::SetFrontWheels(double rate)
{
	if(rate != 0)
	{
		mFrontSteer->Set(rate);
	}
	else
	{
		mFrontSteer->Disable();
	}
}

void CrabDriveSteer::SetBackWheels(double rate)
{
	if(rate != 0)
	{
		mBackSteer->Set(rate);
	}
	else
	{
		mBackSteer->Disable();
	}
}

void CrabDriveSteer::SetTargetHeading(void)
{
	mTargetHeading = GetRobotHeading();
}

void CrabDriveSteer::StopAllWheels(void)
{
	mBackSteer->Disable();
	mFrontSteer->Disable();
}

void CrabDriveSteer::SteerFrontWheels(double angle)
{
	double heading = GetFrontHeading();
	double direction = 0;
	double turnAngle = NormalizeAngle(angle - heading);

	if(turnAngle < 90)
	{
		direction = kCCW;
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}
	else if(turnAngle < 180)
	{
		direction = kCW;
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		angle = NormalizeAngle(angle + 180);
	}
	else if(turnAngle < 270)
	{
		direction = kCCW;
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		angle = NormalizeAngle(angle + 180);
	}
	else if(turnAngle < 360)
	{
		direction = kCW;
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}

	turnAngle = NormalizeAngle(angle - heading);

	if(turnAngle < MAX_ANGLE_TOLERANCE) //&& (double)mBackEncoder->GetRaw() / 2000.0 * 360.0 != angle)
	{
		//printf("FrontEncoder = %d  Angle = %f  FrontHeading = %f\n", mFrontEncoder->GetRaw(), angle, heading); //, mBackEncoder->GetRaw() / 2000 * 360);

		SetFrontWheels(direction);
		mIsFrontTurning = true;
	}
	else
	{
		SetFrontWheels(0.0);
		mIsFrontTurning = false;
	}
}

void CrabDriveSteer::SteerBackWheels(double angle)
{
	double heading = GetBackHeading();
	double direction = 0;
	double turnAngle = NormalizeAngle(angle - heading);

	if(turnAngle < 90)
	{
		direction = kCCW;
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}
	else if(turnAngle < 180)
	{
		direction = kCW;
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		angle = NormalizeAngle(angle + 180);
	}
	else if(turnAngle < 270)
	{
		direction = kCCW;
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		angle = NormalizeAngle(angle + 180);
	}
	else if(turnAngle < 360)
	{
		direction = kCW;
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}

	turnAngle = NormalizeAngle(angle - heading);

	if(turnAngle < MAX_ANGLE_TOLERANCE) //&& (double)mBackEncoder->GetRaw() / 2000.0 * 360.0 != angle)
	{
		//printf("BackEncoder = %d  Angle = %f  BackHeading = %f\n", mBackEncoder->GetRaw(), angle, heading); //, mBackEncoder->GetRaw() / 2000 * 360);

		SetBackWheels(direction);
		mIsBackTurning = true;
	}
	else
	{
		SetBackWheels(0.0);
		mIsBackTurning = false;
	}
}

void CrabDriveSteer::SteerAllWheels(double angle)
{
	double frontHeading = GetFrontHeading();
	double backHeading = GetBackHeading();
	SteerDirection frontDirection = kStop;
	SteerDirection backDirection = kStop;
	double backTurnAngle = NormalizeAngle(angle - backHeading);
	double frontTurnAngle = NormalizeAngle(angle - frontHeading);
	double minSpeed;
	minSpeed = .175;
	double midSpeed = .5;
	double maxSpeed = 1;
	int FD, LD, RD, BD;
	FD = LD = RD = BD =0;

	if(frontTurnAngle >= 0 && frontTurnAngle < 90)
	{
		frontDirection = kCCW;
		mLeftDirection = kForward;
		mRightDirection = kForward;
		if(backTurnAngle < 180)
		{
			backDirection = kCCW;
		}
		else
		{
			backDirection = kCW;
		}
	}
	else if(frontTurnAngle < 180)
	{
		frontDirection = kCW;
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		angle = NormalizeAngle(angle + 180);
		if(backTurnAngle < 180)
		{
			backDirection = kCW;
		}
		else
		{
			backDirection = kCCW;
		}
	}
	else if(frontTurnAngle < 270)
	{
		frontDirection = kCCW;
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		angle = NormalizeAngle(angle + 180);
		if(backTurnAngle < 180)
		{
			backDirection = kCW;
		}
		else
		{
			backDirection = kCCW;
		}
	}
	else if(frontTurnAngle < 360)
	{
		frontDirection = kCW;
		mLeftDirection = kForward;
		mRightDirection = kForward;
		if(backTurnAngle < 180)
		{
			backDirection = kCCW;
		}
		else
		{
			backDirection = kCW;
		}
	}


	/*frontDirection = GetFrontSteerDirection(frontTurnAngle);
	backDirection = GetBackSteerDirection(backTurnAngle);

	if(mLeftDirection == kReverse && mRightDirection == kReverse)
	{
		angle = NormalizeAngle(angle + 180);
	}*/

	FD = frontDirection;
	BD = backDirection;
	RD = mRightDirection;
	LD = mLeftDirection;

	backTurnAngle = 180 - fabs(fabs( angle - backHeading) - 180);
	frontTurnAngle = 180 - fabs(fabs( angle - frontHeading) - 180);
	//printf("Front Turn Angle: %f   Back Turn Angle: %f  ", frontTurnAngle, backTurnAngle);

	if(frontTurnAngle > MAX_ANGLE_TOLERANCE)
	{
		//printf("Angle = %f  BackHeading = %f\n", angle, backHeading); //, mBackEncoder->GetRaw() / 2000 * 360);
		mIsFrontTurning = true;
		SetFrontWheels(maxSpeed * frontDirection * mFrontPConstant);
		//SetBackWheels(speed * frontDirection);
	}
	else if(frontTurnAngle > MID_ANGLE_TOLERANCE)
	{
		mIsFrontTurning = true;
		SetFrontWheels(midSpeed * frontDirection * mFrontPConstant);
	}
	else if(frontTurnAngle > MIN_ANGLE_TOLERANCE)
	{
		mIsFrontTurning = true;
		SetFrontWheels(.2 * frontDirection);
	}
	else
	{
		SetFrontWheels(0.0);
		mIsFrontTurning = false;
	}

	if(backTurnAngle > MAX_ANGLE_TOLERANCE)
	{
		//printf("Angle = %f  BackHeading = %f\n", angle, backHeading); //, mBackEncoder->GetRaw() / 2000 * 360);
		mIsBackTurning = true;
		SetBackWheels(maxSpeed * backDirection * mBackPConstant);
		//SetBackWheels(speed * frontDirection);
	}
	else if(backTurnAngle > MID_ANGLE_TOLERANCE)
	{
		mIsBackTurning = true;
		SetBackWheels(midSpeed * backDirection * mBackPConstant);
	}
	else if(backTurnAngle > MIN_ANGLE_TOLERANCE)
	{
		mIsBackTurning = true;
		SetBackWheels(.2 * backDirection);
	}
	else
	{
		SetBackWheels(0.0);
		mIsBackTurning = false;
	}
}

bool CrabDriveSteer::TightTurn(double angle)
{
	SteerDirection frontDirection = kStop;
	SteerDirection backDirection = kStop;
	double frontAngle = NormalizeAngle(GetRobotHeading() - angle - 90);
	double backAngle = NormalizeAngle(GetRobotHeading() + angle - 90);
	double frontHeading = GetFrontHeading();
	double backHeading = GetBackHeading();
	double frontTurnAngle = fabs(frontAngle - frontHeading);
	if(frontTurnAngle > 180)
		frontTurnAngle = 360 - frontTurnAngle;
	double backTurnAngle = fabs(backAngle - backHeading);
	if(backTurnAngle > 180)
		backTurnAngle = 360 - backTurnAngle;
	double frontForward, frontReverse, forwardMax, backForward, backReverse, reverseMax;
	frontForward = frontTurnAngle;
	frontReverse = 180 - frontTurnAngle;
	backForward = backTurnAngle;
	backReverse = 180 - backTurnAngle;

	if(frontForward < backForward)
		forwardMax = backForward;
	else
		forwardMax = frontForward;

	if(frontReverse < backReverse)
		reverseMax = backReverse;
	else
		reverseMax = frontReverse;

	if(forwardMax < reverseMax)
	{
		frontTurnAngle = NormalizeAngle(frontAngle - frontHeading);
		backTurnAngle = NormalizeAngle(backAngle - backHeading);
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}
	else
	{
		frontTurnAngle = NormalizeAngle(frontAngle - frontHeading);
		backTurnAngle = NormalizeAngle(backAngle - backHeading);
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
	}

	if(frontTurnAngle >= 0 && frontTurnAngle < 90)
	{
		frontDirection = kCCW;
	}
	else if(frontTurnAngle < 180)
	{
		frontDirection = kCW;
		frontAngle = NormalizeAngle(frontAngle + 180);
	}
	else if(frontTurnAngle < 270)
	{
		frontDirection = kCCW;
		frontAngle = NormalizeAngle(frontAngle + 180);
	}
	else if(frontTurnAngle < 360)
	{
		frontDirection = kCW;
	}

	if(backTurnAngle >= 0 && backTurnAngle < 90)
	{
		backDirection = kCCW;
	}
	else if(backTurnAngle < 180)
	{
		backDirection = kCW;	
		backAngle = NormalizeAngle(backAngle + 180);
	}
	else if(backTurnAngle < 270)
	{
		backDirection = kCCW;
		backAngle = NormalizeAngle(backAngle + 180);
	}
	else if(backTurnAngle < 360)
	{
		backDirection = kCW;
	}

	backTurnAngle = 180 - fabs(fabs(backAngle - backHeading) - 180);
	frontTurnAngle = 180 - fabs(fabs(frontAngle - frontHeading) - 180);

	if(frontTurnAngle > MAX_ANGLE_TOLERANCE)
	{
		//printf("Angle = %f  BackHeading = %f\n", angle, backHeading); //, mBackEncoder->GetRaw() / 2000 * 360);
		mIsFrontTurning = true;
		SetFrontWheels(1 * frontDirection);
		//SetBackWheels(speed * frontDirection);
	}
	else if(frontTurnAngle > 5)
	{
		mIsFrontTurning = true;
		SetFrontWheels(.4 * frontDirection);
	}
	else
	{
		SetFrontWheels(0.0);
		mIsFrontTurning = false;
	}

	if(backTurnAngle > MAX_ANGLE_TOLERANCE)
	{
		//printf("Angle = %f  BackHeading = %f\n", angle, backHeading); //, mBackEncoder->GetRaw() / 2000 * 360);
		mIsBackTurning = true;
		SetBackWheels(1 * backDirection);
		//SetBackWheels(speed * frontDirection);
	}
	else if(backTurnAngle > 5)
	{
		mIsBackTurning = true;
		SetBackWheels(.4 * backDirection);
	}
	else
	{
		SetBackWheels(0.0);
		mIsBackTurning = false;
	}

	if(!mIsFrontTurning && !mIsBackTurning)
	{
		return true;
	}
	else
	{
		return false;
	}

}

void CrabDriveSteer::AdjustSteer(void)
{
	double frontDistance, backDistance;
	frontDistance = fabs(mFrontEncoder->GetRate());
	backDistance = fabs(mBackEncoder->GetRate());

	if(frontDistance > 0 && backDistance > 0)
	{
		if(frontDistance < backDistance)
		{
			mBackPConstant = frontDistance / backDistance;
			mFrontPConstant = 1;
		}
		else if(backDistance < frontDistance)
		{
			mFrontPConstant = backDistance / frontDistance;
			mBackPConstant = 1;
		}
	}

	printf("BackConstant = %.3f  FrontConstant = %.3f\n", mBackPConstant, mFrontPConstant);
}

bool CrabDriveSteer::AdjustHeading(void)
{
	double currentHeading;
	currentHeading = GetRobotHeading();

	if(fabs(mTargetHeading - currentHeading) > 5)
	{
		if(currentHeading < mTargetHeading)
			mSpinDirection = 1;
		else
			mSpinDirection = -1;
		return true;
	}
	else
	{
		return false;
	}
}

double CrabDriveSteer::GetFrontHeading(void)
{
	double heading;
	int normalizeEncoder = mFrontEncoder->GetRaw()%TICKS_PER_WHEEL_REVOLUTION;
	if(normalizeEncoder < 0)
	{
		normalizeEncoder = normalizeEncoder + TICKS_PER_WHEEL_REVOLUTION;
	}
	heading = NormalizeAngle((double)normalizeEncoder / (double)TICKS_PER_WHEEL_REVOLUTION * 360.0 - mGyro->GetAngle() + 90.0);
	return(heading);
}

double CrabDriveSteer::GetBackHeading(void)		
{
	double heading;
	int normalizeEncoder = mBackEncoder->GetRaw()%TICKS_PER_WHEEL_REVOLUTION;		
	if(normalizeEncoder < 0)
	{
		normalizeEncoder = normalizeEncoder + TICKS_PER_WHEEL_REVOLUTION;
	}
	heading = NormalizeAngle((double)normalizeEncoder / (double)TICKS_PER_WHEEL_REVOLUTION * 360.0 - mGyro->GetAngle() + 90.0);
	return(heading);
}

double CrabDriveSteer::GetRobotHeading(void)
{
	double angle;
	angle = 360 - mGyro->GetAngle() + 90;

	angle = NormalizeAngle(angle);

	return(angle);
}

void CrabDriveSteer::GetDriveDirection(double turnAngle)
{
	if(turnAngle >= 0 && turnAngle < 90)
	{
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}
	else if(turnAngle < 180)
	{
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
	}
	else if(turnAngle < 270)
	{
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
	}
	else if(turnAngle < 360)
	{
		mLeftDirection = kForward;
		mRightDirection = kForward;
	}
}

CrabDriveSteer::SteerDirection CrabDriveSteer::GetSteerDirection(double turnAngle)
{

	if(turnAngle >= 0 && turnAngle < 90)
	{
		return(kCCW);
	}
	else if(turnAngle < 180)
	{
		return(kCW);
	}
	else if(turnAngle < 270)
	{
		return(kCCW);
	}
	else if(turnAngle < 360)
	{
		return(kCW);
	}
	else
	{
		return(kStop);
	}
}

CrabDriveSteer::DriveDirection CrabDriveSteer::GetLeftDirection(void)
{
	return mLeftDirection;
}

CrabDriveSteer::DriveDirection CrabDriveSteer::GetRightDirection(void)
{
	return mRightDirection;
}

void CrabDriveSteer::ResetEncoder(void)
{
	mFrontEncoder->Reset();
	mBackEncoder->Reset();
	mGyro->Reset();
}

void CrabDriveSteer::ResetGyro(void)
{
	mGyro->Reset();
}

void CrabDriveSteer::ResetWheels(void)
{
	mFrontCounter->Reset();
	mFrontEncoder->Reset();
	mFrontCounter->Start();
	mFrontEncoder->Start();
	while(mFrontEncoder->GetRaw() < 10000)
	{
		mFrontSteer->Set(.4);
		//printf("Encoder = %f  Counter = %f\n", mFrontEncoder->GetRaw(),mFrontCounter->Get());
		//Wait(.01);
	}
	mFrontSteer->Set(0.0);
	//printf("Encoder = %d  Counter = %d\n", mFrontEncoder->GetRaw(),mFrontCounter->Get());
	mFrontCounter->Stop();
	mFrontEncoder->Stop();
	while(mBackCounter->Get() != 1)
	{
		mBackSteer->Set(.1);
		Wait(.01);
	}
	mBackSteer->Set(0.0);
	mBackCounter->Stop();
}

double CrabDriveSteer::NormalizeAngle(double angle)
{
	angle = angle - int(angle / 360.0) * 360.0;

	if(angle < 0)
	{
		angle = angle + 360.0;
	}

	return(angle);
}

void CrabDriveSteer::UpdateSmartDashboard(double angle)
{
	double frontTurnAngle = NormalizeAngle(angle - GetFrontHeading());
	double backTurnAngle = NormalizeAngle(angle - GetBackHeading());
	SmartDashboard::PutNumber("Front Heading", GetFrontHeading());
	SmartDashboard::PutNumber("Back Heading", GetBackHeading());
	SmartDashboard::PutNumber("Angle", angle);
	SmartDashboard::PutNumber("Front Turn Angle", frontTurnAngle);
	SmartDashboard::PutNumber("Back Turn Angle", backTurnAngle);
	SmartDashboard::PutBoolean("Is Front Turning", mIsFrontTurning);
	SmartDashboard::PutBoolean("Is Back Turning", mIsBackTurning);
	SmartDashboard::PutNumber("Right Drive Direction", mRightDirection);
	SmartDashboard::PutNumber("Left Drive Direction", mLeftDirection);
	SmartDashboard::PutNumber("Front Encoder", mFrontEncoder->GetRaw());
	SmartDashboard::PutNumber("Back Encoder", mBackEncoder->GetRaw());
}

#endif
