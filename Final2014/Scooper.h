#ifndef SCOOPER_H
#define SCOOPER_H

class Scooper
{
public:
	enum ScooperPosition{kLower, kRaise, kRelease};

	Scooper(void);
	~Scooper(void);

	void UpdateScooper(int);
	void StopScooper(void);
	
	long GetPotentiometerValue(void);
	void ResetPotentiometer(void);
	void CalibratePotentiometer(void);
	
	bool GetIsCycleDone(void);

private:
	void RaiseScooper(void);
	void LowerScooper(void);
	void ScooperDriving(void);
	void ScooperPickUp(void);
	void ScooperShooting(void);

	Talon* mScooper;
	AnalogChannel* mScooperPotentiometer;
	Timer* mTimer;

	int mHoldBall;
	
	bool IsCycleDone;
};

Scooper::Scooper(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mScooper = new Talon(MODULE_1, SCOOPER_TALON);
	mScooperPotentiometer = new AnalogChannel(SCOOPER_POTENTIOMETER);
	mTimer = new Timer();
	mTimer->Reset();
	mTimer->Start();
	
	mHoldBall = GetPotentiometerValue();
	
	IsCycleDone = false;
}

Scooper::~Scooper(void)
{
	delete mScooper;
	mScooper = NULL;
	delete mScooperPotentiometer;
	mScooperPotentiometer = NULL;
}

void Scooper::UpdateScooper(int status)
{
	if(status == kLower)
	{
		LowerScooper();
	}
	else if(status == kRaise/* && GetPotentiometerValue() <= mHoldBall + SCOOPER_TOLERANCE*/)
	{
		RaiseScooper();
	}
	else if(status == kRelease)
	{
		ScooperDriving();
	}
	else
	{
		StopScooper();
	}
}

void Scooper::RaiseScooper(void)
{
	mScooper->Set(.4);
	printf("SE GetValue: %d  SE GetAverage: %lu\n", mScooperPotentiometer->GetValue(), GetPotentiometerValue());
}

void Scooper::LowerScooper(void)
{
	mScooper->Set(-.4);
	printf("SE GetValue: %d  SE GetAverage: %lu\n", mScooperPotentiometer->GetValue(), GetPotentiometerValue());
}

void Scooper::ScooperDriving(void)
{
	if(labs(GetPotentiometerValue() - mHoldBall) >= SCOOPER_TOLERANCE)
	{
		mScooper->Set(.4);
	}
	else
	{
		StopScooper();
	}
}

void Scooper::ScooperShooting(void)
{
	if(labs(GetPotentiometerValue() - (mHoldBall + SCOOPER_RELEASE_BALL)) >= SCOOPER_TOLERANCE)
	{
		if(GetPotentiometerValue() < SCOOPER_TOLERANCE)
			mScooper->Set(.4);
		if(GetPotentiometerValue() > SCOOPER_TOLERANCE)
			mScooper->Set(-.4);
		IsCycleDone = false;
	}
	else
	{
		StopScooper();
		IsCycleDone = true;
	}
}

void Scooper::ScooperPickUp(void)
{
	if(GetPotentiometerValue() >= (mHoldBall + SCOOPER_PICK_UP_POSITION) - SCOOPER_TOLERANCE)
	{
		mScooper->Set(-.4);
	}
	else
	{
		StopScooper();
	}
}

bool Scooper::GetIsCycleDone(void)
{
	return IsCycleDone;
}

void Scooper::StopScooper(void)
{
	mScooper->Disable();
}

long Scooper::GetPotentiometerValue(void)
{
	return  mScooperPotentiometer->GetAverageValue();
}

void Scooper::ResetPotentiometer(void)
{
	mHoldBall = GetPotentiometerValue();
}

void Scooper::CalibratePotentiometer(void)
{
	mHoldBall = GetPotentiometerValue() - 300;
}

#endif
