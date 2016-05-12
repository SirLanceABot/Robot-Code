#ifndef LIFTER_H
#define LIFTER_H

class Lifter
{
public:
	enum LifterPosition{kLower, kRaise};

	Lifter(void);
	~Lifter(void);

	void UpdateLifter(int);

private:
	void RaiseLifter(void);
	void LowerLifter(void);

	Compressor* mCompressor;
	Solenoid* mExtendLifter;
	Solenoid* mRetractLifter;

	bool mIsExtended;
	bool mIsRetracted;
};

Lifter::Lifter(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mCompressor = new Compressor(MODULE_1, PRESSURE_SWITCH, MODULE_1, COMPRESSOR_RELAY);
	mCompressor->Start();

	mExtendLifter = new Solenoid (LIFTER_SOLENOID_OUT);
	mRetractLifter = new Solenoid (LIFTER_SOLENOID_IN);

	mExtendLifter->Set(false);
	mRetractLifter->Set(true);

	mIsExtended = false;
	mIsRetracted = true;
}

Lifter::~Lifter(void)
{
	delete mCompressor;
	mCompressor = NULL;
	delete mExtendLifter;
	mExtendLifter = NULL;
	delete mRetractLifter;
	mRetractLifter = NULL;
}

void Lifter::UpdateLifter(int status)
{
	if(status == kLower && mIsExtended)
	{
		LowerLifter();
	}

	if(status == kRaise && mIsRetracted)
	{
		RaiseLifter();
	}
}

void Lifter::RaiseLifter(void)
{
	mRetractLifter->Set(false);
	mExtendLifter->Set(true);

	mIsExtended = true;
	mIsRetracted = false;
}

void Lifter::LowerLifter(void)
{
	mExtendLifter->Set(false);
	mRetractLifter->Set(true);

	mIsExtended = false;
	mIsRetracted = true;
}

#endif
