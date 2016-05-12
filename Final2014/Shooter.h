#ifndef SHOOTER_H
#define SHOOTER_H

class Shooter
{
public:
	enum ShooterInstruction{kDoNotShoot, kShoot, kManualUp, kManualDown, kPass};

	Shooter(void);
	~Shooter(void);

	void UpdateShooter(int);
	void StopShooter(void);
	void ResetEncoder(void);

	bool GetIsCycleDone(void);

	int GetFrontSwitch(void);

	/*	void ManualShoot(int direction);*/

private:
	void StartShoot(void);
	void StartPass(void);
	void StartReload(void);

	Talon* mBigShooter;
	Talon* mLittleShooter;
	DigitalInput* mFrontSwitch;
	Encoder* mShooterEncoder;
	LogitechJoystick* mLogitech;
	Timer *mTimer;

	bool mIsShooting;
	bool mIsCycleDone;
	bool mIsReloading;
};

Shooter::Shooter()
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mBigShooter = new Talon(MODULE_1, BIG_SHOOTER_TALON);
	mLittleShooter = new Talon(MODULE_1, LITTLE_SHOOTER_TALON);
	mFrontSwitch = new DigitalInput(MODULE_1, SHOOTER_SWITCH);
	mLogitech = new LogitechJoystick();

	mShooterEncoder = new Encoder(MODULE_1, SHOOTER_ENCODER_A, MODULE_1, SHOOTER_ENCODER_B, true, Encoder::k4X);
	mShooterEncoder->Reset();
	mShooterEncoder->Start();
	
	mTimer = new Timer();
	mTimer->Reset();

	mIsShooting = false;
	mIsReloading = false;
	mIsCycleDone = false;
}

Shooter::~Shooter()
{
	delete mBigShooter;
	mBigShooter = NULL;
	delete mLittleShooter;
	mLittleShooter = NULL;
	delete mFrontSwitch;
	mFrontSwitch = NULL;
	delete mShooterEncoder;
	mShooterEncoder = NULL;
}

void Shooter::ResetEncoder(void)
{
	mShooterEncoder->Reset();
}

void Shooter::UpdateShooter(int status)
{
	if((status == kShoot) && (mIsReloading == false && mIsShooting == false /*&& mFrontSwitch->Get() == 0*/))
	{
		//printf("UpdateShooter - Start Shooting\n");
		mTimer->Reset();
		mTimer->Start();
		StartShoot();

		mIsShooting = true;
		mIsCycleDone = false;
	}
	
	if((status == kPass) && (mIsReloading == false && mIsShooting == false /*&& mFrontSwitch->Get() == 0*/))
	{
		mTimer->Reset();
		mTimer->Start();
		StartPass();
		
		mIsShooting = true;
		mIsCycleDone = false;
	}

	if(mIsShooting == true && (/*mFrontSwitch->Get() == 1 ||*/ /*mShooterEncoder->GetRaw() > SHOOTER_SHOOT_STOP || */mTimer->Get() >= .25))
	{
		//printf("UpdateShooter - Stop Shooting, Start Reloading\n");
		//printf("Shooter Rate: %f", mShooterEncoder->GetRate());

		StopShooter();

		StartReload();
		mTimer->Reset();
		mTimer->Start();

		mIsShooting = false;
		mIsReloading = true;
	}

	if(mIsReloading == true && mTimer->Get() >= 5)
	{
		//printf("UpdateShooter - Stop Reloading\n");

		StopShooter();
		mTimer->Stop();
		mTimer->Reset();

		mIsReloading = false;

		Wait(0.2);

		ResetEncoder();

		mIsCycleDone = true;
	}

	if(status == kManualUp)
	{
		mBigShooter->Set(-.1);
		mLittleShooter->Set(-.1);
	}
	if(status == kManualDown)
	{
		mBigShooter->Set(.1);
		mLittleShooter->Set(.1);
	}

	printf("Encoder: %ld\n", mShooterEncoder->GetRaw());
}

void Shooter::StartShoot(void)
{
	//printf("Start Shoot\n");
	//double speed = (mLogitech->GetAxis(LOGITECH_THROTTLE) - 1)/2;
	mBigShooter->Set(-1.0);
	mLittleShooter->Set(-1.0);
}

void Shooter::StartPass(void)
{
	mBigShooter->Set(-0.6);
	mLittleShooter->Set(-0.6);
}

void Shooter::StartReload(void)
{
	//printf("Start Reload\n");

	mLittleShooter->Set(0.2);
	mBigShooter->Set(0.2);

	Wait(0.06);

	mLittleShooter->Set(0.1);
//	mBigShooter->Set(0.1);
}

void Shooter::StopShooter(void)
{
	mLittleShooter->Disable();
	mBigShooter->Disable();
}

bool Shooter::GetIsCycleDone(void)
{
	return mIsCycleDone;
}

int Shooter::GetFrontSwitch(void)
{
	return mFrontSwitch->Get();
}

/*void Shooter::ManualShoot(int direction)
{
	mBigShooter->Set(.2 * direction);
	mLittleShooter->Set(.2 * direction);
}*/

#endif
