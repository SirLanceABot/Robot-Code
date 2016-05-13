#ifndef CRAB_DRIVE_STEER_H
#define CRAB_DRIVE_STEER_H

class CrabDriveSteer
{
public:
	enum SteerDirection{kCCW = -1, kCW = +1, kStop = 0};
	enum DriveDirection{kForward = 1, kReverse = -1, kOff = 0};
	CrabDriveSteer(void);
	~CrabDriveSteer(void);
	void UpdateSteer(double);
	void SetFrontWheels(double);
	void SetBackWheels(double);
	void StopAllWheels(void);
	void SteerFrontWheels(double);
	void SteerBackWheels(double);
	void SteerAllWheels(double);
	void ResetWheels(void);
	double GetFrontHeading(void);
	double GetBackHeading(void);
	double GetRobotHeading(void);
	SteerDirection GetFrontSteerDirection(double);
	SteerDirection GetBackSteerDirection(double);
	void ResetEncoder(void);
	DriveDirection GetRightDirection(void);
	DriveDirection GetLeftDirection(void);
	void CalibrateWheels(void);
	double NormalizeAngle(double);
	void SpinRobot(double angle);
	bool TightTurn(double);
	void UpdateSmartDashboard(double);
	void Print(void);
	long int GetGyroTemperature(void);
	long int GetAnalogVoltage(void);
	void GyroGetValue(void);

	private:
	Talon* mFrontSteer;
	Talon* mBackSteer;
	Encoder* mFrontEncoder;
	Encoder* mBackEncoder;
	Counter* mFrontCounter;
	Counter* mBackCounter;

	Gyro2014* mGyro;
	AnalogChannel* mGyroTemperature;
	AnalogChannel* mChannelVoltage;

	DriveDirection mLeftDirection;
	DriveDirection mRightDirection;

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

	mGyro = new Gyro2014(GYRO_ANALOG_PORT);
	mGyro->SetSensitivity(Gyro2014::kDefaultVoltsPerDegreePerSecond*GYRO_SENSITIVITY_AJUSTMENT);
	mGyro->Reset();
	printf("Made the Gyro in CrabDriveSteer\n\n");
	mGyro->Print();
	printf("Printed the Gyro in CrabDriveSteer\n\n");

	mGyroTemperature = new AnalogChannel(GYRO_TEMPERATURE_PORT);
	mChannelVoltage  = new AnalogChannel(3);

	mLeftDirection = kForward;
	mRightDirection = kForward;

	mIsFrontTurning = false;
	mIsBackTurning = false;
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
	/*
		SteerAllWheels(angle);
		SteerFrontWheels(angle);
		SteerBackWheels(angle);
	*/
	//printf("BackEncoder = %d  Front Encoder = %d \n", mBackEncoder->GetRaw(), mFrontEncoder->GetRaw());
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

void CrabDriveSteer::StopAllWheels(void)
{
	mBackSteer->Disable();
	mFrontSteer->Disable();
}

void CrabDriveSteer::SteerFrontWheels(double angle)	// not checked by RKT
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
	}
	else
	{
		SetFrontWheels(0.0);
	}
}

void CrabDriveSteer::SteerBackWheels(double angle)	// not checked by RKT
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
	}
 	else
	{
		SetBackWheels(0.0);
	}
}

void CrabDriveSteer::SteerAllWheels(double angle) // desired angle with respect to the field without rotating the chassis through this action (others may rotate the chassis)
{
	double minSpeed = .25;
	double midSpeed = .5;
	double maxSpeed = .75;

	SteerDirection frontDirection;
	SteerDirection backDirection;

	double frontHeading, backHeading;

	double frontTurnAngle, backTurnAngle;

	frontHeading = GetFrontHeading();  // wheels currently pointing forward in this field direction
	backHeading = GetBackHeading();
	printf("  FH: %f,  BH: %f  ",frontHeading, backHeading);

	frontDirection = kCCW;
	frontTurnAngle = angle - frontHeading; // CCW angle; (kCCW or negative motor power)
	if (frontTurnAngle <   0.) // normalized angle still CCW
		{
		frontTurnAngle = 360. + frontTurnAngle;
		}
	
	if (frontTurnAngle > 180.) // CCW angle large so CW angle is smaller so use that path
		{
		frontDirection = kCW;
		frontTurnAngle = 360. - frontTurnAngle;
		}

	backDirection = kCCW;
	backTurnAngle = angle - backHeading; // CCW angle; (kCCW or negative motor power)
	if (backTurnAngle <   0.) // normalized angle still CCW
		{
		backTurnAngle = 360. + backTurnAngle;
		}

	if (backTurnAngle > 180.) // CCW angle large so CW angle is smaller so use that path
		{
		backDirection = kCW;
		backTurnAngle = 360. - backTurnAngle;
		}

	if (frontTurnAngle < 90.)
	{
		mLeftDirection = mRightDirection = kForward;
	}
	else // large front turn angle so reverse drive is shorter steer turning path for the front; front and back drive together
	{
		mLeftDirection = mRightDirection = kReverse; // reverse driving direction; force right drive same as left
		frontTurnAngle = fabs(frontTurnAngle - 180.); // reverse steering angle
		backTurnAngle  = fabs(backTurnAngle  - 180.);
		frontDirection = frontDirection == kCCW? kCW: kCCW; // flip steering direction to achieve angle since driving is reversed
		backDirection  = backDirection  == kCCW? kCW: kCCW;
		}

	//printf("Front Turn Angle: %f   Back Turn Angle: %f  ", frontTurnAngle, backTurnAngle);

	// modicum of proportional control on steering speed to help prevent much overshoot
	if(fabs(frontTurnAngle) > MAX_ANGLE_TOLERANCE)
	{
		SetFrontWheels(maxSpeed * frontDirection);
	}
	else if(fabs(frontTurnAngle) > MID_ANGLE_TOLERANCE)
	{
		SetFrontWheels(midSpeed * frontDirection);
	}
	else if(fabs(frontTurnAngle) > MIN_ANGLE_TOLERANCE)
	{
		SetFrontWheels(minSpeed * frontDirection);
	}
	else
	{
//		printf(" stop front turn ");
		SetFrontWheels(0.0);
	}
	
	if(fabs(backTurnAngle) > MAX_ANGLE_TOLERANCE)
	{
		SetBackWheels(maxSpeed * backDirection);
	}
	else if(fabs(backTurnAngle) > MID_ANGLE_TOLERANCE)
	{
		SetBackWheels(midSpeed * backDirection);
	}
	else if(fabs(backTurnAngle) > MIN_ANGLE_TOLERANCE)
	{
		SetBackWheels(minSpeed * backDirection);
	}
	else
	{
//		printf(" stop back turn ");
		SetBackWheels(0.0);
	}
}

void CrabDriveSteer::ResetWheels(void)	// not checked by RKT
{
	mFrontCounter->Reset();
	mFrontEncoder->Reset();
	mFrontCounter->Start();
	mFrontEncoder->Start();

	while(mFrontEncoder->GetRaw() < 10000)
	{
		mFrontSteer->Set(.4);
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

double CrabDriveSteer::GetFrontHeading(void)
{
	double heading;
	int normalizeEncoder = mFrontEncoder->GetRaw()%TICKS_PER_WHEEL_REVOLUTION;
	heading = NormalizeAngle((double)normalizeEncoder / (double)TICKS_PER_WHEEL_REVOLUTION * 360.0 + GetRobotHeading());
	return(heading);
}

double CrabDriveSteer::GetBackHeading(void)		
{
	double heading;
	int normalizeEncoder = mBackEncoder->GetRaw()%TICKS_PER_WHEEL_REVOLUTION;		
	heading = NormalizeAngle((double)normalizeEncoder / (double)TICKS_PER_WHEEL_REVOLUTION * 360.0 + GetRobotHeading());
	return(heading);
}

double CrabDriveSteer::GetRobotHeading(void)
{
	double angle;

	angle = mGyro->GetAngle();

	angle = 360. - NormalizeAngle(angle);

	return(angle);
}

CrabDriveSteer::SteerDirection CrabDriveSteer::GetFrontSteerDirection(double turnAngle)	// not checked by RKT
{
	if(turnAngle >= 0 && turnAngle < 90)
	{
		mLeftDirection = kForward;
		mRightDirection = kForward;
		return(kCCW);
	}
	else if(turnAngle < 180)
	{
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		return(kCW);
	}
	else if(turnAngle < 270)
	{
		mLeftDirection = kReverse;
		mRightDirection = kReverse;
		return(kCCW);
	}
	else if(turnAngle < 360)
	{
		mLeftDirection = kForward;
		mRightDirection = kForward;
		return(kCW);
	}
	else
	{
		return(kStop);
	}
}

CrabDriveSteer::SteerDirection CrabDriveSteer::GetBackSteerDirection(double turnAngle)	// not checked by RKT
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

void CrabDriveSteer::ResetEncoder(void)
{
	mFrontEncoder->Reset();
	mBackEncoder->Reset();
}

CrabDriveSteer::DriveDirection CrabDriveSteer::GetRightDirection(void)
{
	return mRightDirection;
}

CrabDriveSteer::DriveDirection CrabDriveSteer::GetLeftDirection(void)
{
	return mLeftDirection;
}

void CrabDriveSteer::CalibrateWheels(void)
{
	if(mXBoxJoystick->GetXBoxButton(XBoxJoystick::kA))	SetFrontWheels(0.3);
	else if(mXBoxJoystick->GetXBoxButton(XBoxJoystick::kB)) SetFrontWheels(-0.3);
	else if(mXBoxJoystick->GetXBoxButton(XBoxJoystick::kX)) SetBackWheels(0.3);
	else if(mXBoxJoystick->GetXBoxButton(XBoxJoystick::kY)) SetBackWheels(-0.3);
	else if(mXBoxJoystick->GetXBoxButton(XBoxJoystick::kBack)) mGyro->Reset();
	else if(mXBoxJoystick->GetXBoxButton(XBoxJoystick::kStart)) ResetEncoder();
	else
	{
		SetBackWheels(0.0);
		SetFrontWheels(0.0);
	}

	printf("\nFront Encoder: %ld  Back Encoder: %ld  Gyro Angle: %f\n", mFrontEncoder->GetRaw(), mBackEncoder->GetRaw(), mGyro->GetAngle());
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

void CrabDriveSteer::SpinRobot(double angle)	// not checked by RKT
{
	double heading = GetRobotHeading();
	double turnAngle = NormalizeAngle(angle - heading);
	
	if(turnAngle >= 0 && turnAngle <= 180)
	{
		mLeftDirection = kReverse;
		mRightDirection = kForward;
	}
	if(turnAngle > 180 && turnAngle <= 360)
	{
		mLeftDirection = kForward;
		mRightDirection = kReverse;
	}
}

bool CrabDriveSteer::TightTurn(double angle)	// not checked by RKT
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

void CrabDriveSteer::Print(void)
	{

	printf("EF:,%ld, EB:,%ld, ", mFrontEncoder->GetRaw(), mBackEncoder->GetRaw());
	printf("CF:,%ld, CB:,%ld, ", mFrontCounter->Get(), mBackCounter->Get());
	printf("GA:,%f,  GR:,%f,",mGyro->GetAngle(), mGyro->GetRate());

	}

long int CrabDriveSteer::GetGyroTemperature(void)
{
	return mGyroTemperature->GetValue(); // or GetAverageValue
}

long int CrabDriveSteer::GetAnalogVoltage(void)
{
	return mChannelVoltage->GetValue(); // or GetAverageValue
}

void CrabDriveSteer::GyroGetValue(void)
{
	mGyro->GetValue();
}

#endif
