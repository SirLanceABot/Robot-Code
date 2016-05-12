#ifndef ARM_H
#define ARM_H

class Arm
{
public:
	Arm(Encoder*, LHSJoystick*, Talon*, DigitalInput*, DigitalInput*);
	~Arm();
	void UpdateArm();
	bool ClawOut();
	bool ClawIn();
	bool PickUpContainer(); //not here yet
	bool TestArm();
	void Calibrate();

private:
	LHSJoystick* mXbox;
	Talon* mArmMotor;
	Talon* mClawMotor;
	Encoder* mArmEncoder;
	DigitalInput* mHallEffectOut;
	DigitalInput* mHallEffectIn;
	Timer* mTimer;
	int mStep;
	bool mClawOut;
	bool mClawIn;
};

Arm::Arm(Encoder* pArmEncoder, LHSJoystick* pXbox, Talon* pArmMotor, DigitalInput* pHallEffectOut, DigitalInput* pHallEffectIn)
{
	mXbox = pXbox;
	mArmMotor = pArmMotor;
	mClawMotor = new Talon(HOOK_TALON); //undecided
	mArmEncoder = pArmEncoder;
	mHallEffectOut = pHallEffectOut;
	mHallEffectIn = pHallEffectIn;
	mTimer = new Timer();
	mStep = 1;
	mClawOut = true;
	mClawIn = true;
}

Arm::~Arm()
{
	delete mTimer;
	mTimer = NULL;

	delete mHallEffectIn;
	mHallEffectIn = NULL;
	delete mHallEffectOut;
	mHallEffectOut = NULL;


	delete mArmEncoder;
	mArmEncoder = NULL;
	delete mClawMotor;
	mClawMotor = NULL;
	delete mArmMotor;
	mArmMotor = NULL;

	delete mXbox;
	mXbox = NULL;
}

void Arm::Calibrate()
{
	Wait(1.0);
	mClawMotor->Set(1.0);
	Wait(2.0);
	mClawMotor->Set(-1.0);
	Wait(2.0);
	mClawMotor->Set(0.0);
	Wait(1.0);
}

void Arm::UpdateArm()
{
	int POV = mXbox->GetPOV();
	int encoderVal = mArmEncoder->GetRaw() * -1;
	printf("Encoder Value: %d\n", encoderVal);

	/***** OVERRIDE MODE *****/
	if(mXbox->GetRawButton(XBOX_A_BUTTON))
	{
		/***** RESETS ARM ENCODER IF START BUTTON IS PRESSED *****/
		if(mXbox->GetRawButton(XBOX_START_BUTTON))
		{
			mArmEncoder->Reset();
		}
		/***** SETS ARM TO GO UP IF D-PAD IS PRESSED UP *****/
		if(POV == 0)
		{
			mArmMotor->Set(1);
		}
		/***** SETS ARM TO GO DOWN IF D-PAD IS PRESSED DOWN *****/
		else if(POV == 180)
		{
			mArmMotor->Set(-1);
		}
		else
		{
			mArmMotor->Set(0.0);
		}
	}

	else
	{
		/***** RESETS ARM ENCODER IF START BUTTON IS PRESSED *****/
		if(mXbox->GetRawButton(XBOX_START_BUTTON))
		{
			mArmEncoder->Reset();
		}
		/***** SETS ARM MOTOR TO GO FORWARD IF D-PAD IS PRESSED UP *****/
		if(POV == 0 && (encoderVal >= 0))
		{
			mArmMotor->Set(.5);
		}
		/***** SETS ARM MOTOR TO GO BACKWARDS IF D-PAD IS PRESSED DOWN *****/
		else if(POV == 180 && (encoderVal <= 800))
		{
			mArmMotor->Set(-.5);
		}
		else
		{
			mArmMotor->Set(0.0);
		}
	}
	if(POV == 90 && mHallEffectOut->Get() == true)
	{
		mClawMotor->Set(-0.25);
	}
	else if(POV == 270 && mHallEffectIn->Get() == true)
	{
		mClawMotor->Set(0.25);
	}
	else
	{
		mClawMotor->Set(0);
	}
}

bool Arm::PickUpContainer()
{
	bool isDoneLifting = false;

	switch(mStep)
	{
	case 1:
		if(mArmEncoder->GetRaw() * -1 < 800)
		{
			mArmMotor->Set(-0.5);
			return isDoneLifting;
		}
		else
		{
			mArmMotor->Set(0.0);
			mStep = 2;
		}
		break;
	case 2:
		mTimer->Start();
		if(mArmEncoder->GetRaw() * -1 >= 800 && mTimer->Get() < 1) //NEEDS TO BE TESTED
		{
			mClawMotor->Set(1.0/12.0);
			return isDoneLifting;
		}
		else
		{
			mClawMotor->Set(0.0);
			mTimer->Stop();
			mStep = 3;
		}
		break;
	case 3:
		if(mArmEncoder->GetRaw() * -1 > 150)
		{
			mArmMotor->Set(0.5);
			return isDoneLifting;
		}
		else
		{
			mArmMotor->Set(0.0);
			isDoneLifting = true;
			mStep = 4;
		}
		break;
	default:
		return isDoneLifting;
		break;
	}
	return isDoneLifting;
}

bool Arm::TestArm()
{
	//mClawMotor->Set(mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS));
	if(mXbox->GetRawButton(XBOX_X_BUTTON) == true)
		return true;
	if(mXbox->GetRawButton(XBOX_B_BUTTON) == true)
		Calibrate();
	int POV = mXbox->GetPOV();
		int encoderVal = mArmEncoder->GetRaw() * -1;

		/***** OVERRIDE MODE *****/
		if((POV == 0 || POV == 180) && mXbox->GetRawButton(XBOX_A_BUTTON))
		{
			/***** RESETS ARM ENCODER IF START BUTTON IS PRESSED *****/
			if(mXbox->GetRawButton(XBOX_START_BUTTON))
			{
				mArmEncoder->Reset();
			}
			/***** SETS ARM TO GO UP IF D-PAD IS PRESSED UP *****/
			if(POV == 0)
			{
				mArmMotor->Set(1);
			}
			/***** SETS ARM TO GO DOWN IF D-PAD IS PRESSED DOWN *****/
			else if(POV == 180)
			{
				mArmMotor->Set(-1);
			}
			else
			{
				mArmMotor->Set(0.0);
			}
		}

		else
		{
			/***** RESETS ARM ENCODER IF START BUTTON IS PRESSED *****/
			if(mXbox->GetRawButton(XBOX_START_BUTTON))
			{
				mArmEncoder->Reset();
			}
			/***** SETS ARM MOTOR TO GO FORWARD IF D-PAD IS PRESSED UP *****/
			if(POV == 0 && encoderVal >= 0)
			{
				mArmMotor->Set(1);
			}
			/***** SETS ARM MOTOR TO GO BACKWARDS IF D-PAD IS PRESSED DOWN *****/
			else if(POV == 180 && encoderVal <= 800)
			{
				mArmMotor->Set(-1);
			}
			/***** STOPS ARM MOTOR WHEN NOTHING IS PRESSED *****/
			else
			{
				mArmMotor->Set(0.0);
			}
		}
	if(POV == 90)// && mHallEffectOut->Get() == true) //extend
	{
		mClawMotor->Set(-0.25);
	}
	else if(POV == 270)// && mHallEffectIn->Get() == true) //retract
	{
		mClawMotor->Set(0.25);
	}
	else
	{
		mClawMotor->Set(0);
	}

	if(mHallEffectIn->Get() == false)
		printf("Hall Effect In\n");
	if(mHallEffectOut->Get() == false)
		printf("Hall Effect Out\n");

	//		printf("Hall Effect Out: %d     Hall Effect In: %d\n", mHallEffectIn->Get(), mHallEffectOut->Get());
	return false;
}

bool Arm::ClawOut()
{
	if(mHallEffectOut->Get())
	{
		mClawOut = false;
	}
	else
	{
		mClawOut = true;
	}
	return mClawOut;
}
bool Arm::ClawIn()
{
	if(mHallEffectIn->Get())
	{
		mClawIn = false;
	}
	else
	{
		mClawIn = true;
	}
	return mClawIn;
}
#endif
