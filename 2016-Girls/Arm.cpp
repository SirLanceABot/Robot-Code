#include "Arm.h"
#include "WPILib.h"
#include "Constants.h"
#include "XboxJoystick.h"

// TODO:: UPDATE WHEN ARM IS FINISHED

Arm::Arm(int RMPort, int EMPort, int leftWMPort, int rightWMPort, int encoderPort):
mRaiseMotor(std::make_shared<CANTalon>(RMPort)),
mExtendMotor(std::make_shared<CANTalon>(EMPort)),
mLeftWinchMotor(std::make_shared<CANTalon>(leftWMPort)),
mRightWinchMotor(std::make_shared<CANTalon>(rightWMPort)),
mAnalogTrigger(encoderPort)
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
	//	mRaiseMotor = new CANTalon(RMPort);

	//	mVerticalLimit = new DigitalInput(DIO_PORT::VERTICAL_LIMIT); //set port

	//	mLeftWinchMotor = new CANTalon(leftWMPort);
	//	mRightWinchMotor = new CANTalon(rightWMPort);

	mAnalogTrigger.SetLimitsVoltage(3.5, 3.8); // values higher than the highest minimum (pulse floor), lower than the lowest maximum (pulse ceiling)
	mCounter = new Counter(&mAnalogTrigger);
	mSpeedPrevious = 0.;
	mPosition = 0;

	//	mExtendMotor = new CANTalon(EMPort);
	mExtendMotor->ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
	mExtendMotor->ConfigFwdLimitSwitchNormallyOpen(true);
	mExtendMotor->ConfigRevLimitSwitchNormallyOpen(true);

	mRaiseSpeed = 0.5;
	mLowerSpeed = 0.5;
	mExtendSpeed = 0.5;
	mRetractSpeed = 0.5;
	mWinchPullSpeed = 0.5;
	mWinchReleaseSpeed = 0.5;
	mArmStep = kRaisingToVertical;
}

Arm::~Arm()
{
	//	delete mLeftWinchMotor;
	//	delete mRightWinchMotor;
	//	delete mExtendMotor;
	//	delete mRaiseMotor;
}

void Arm::RaiseArm()
{
	mRaiseMotor->Set(CheckDirectionChange(0.8));
}

void Arm::LowerArm()
{
	mRaiseMotor->Set(CheckDirectionChange(-0.4));
}

int Arm::AttachHookAutonomously()
{
	if(mArmStep == kRaisingToVertical) //step 1, raise to vertical
	{
		RaiseArm();
		if(GetPosition() >= 0.25 * 174.9) //stop if verticalLimit switch is hit
		{
			ArmRLStop();
			mArmStep = kExtending;
		}
	}
	else if(mArmStep == kExtending) //step 2, extend the arm all the way up
	{
		ExtendArm();
		if(mExtendMotor->IsFwdLimitSwitchClosed()) //stop if arm is extended all the way up
		{
			ArmERStop();
			mArmStep = kRaisingPastVertical;
		}
	}
	else if(mArmStep == kRaisingPastVertical) //step 3, finished raising the arm to the bar
	{
		RaiseArm();
		if(GetPosition() >= 110.0/360.0 * 174.9) //stop if the arm is all the way raised
		{
			ArmRLStop();
			mArmStep = kRetracting;
		}
	}
	else if(mArmStep == kRetracting) //step 4, retract the arm to place the hook
	{
		RetractArm();
		if(mExtendMotor->IsRevLimitSwitchClosed()) //stop if arm is retracted all the way
		{
			ArmERStop();
			mArmStep = kLowering;
		}
	}
	else if(mArmStep == kLowering) //step 5, lower the arm to resting place
	{
		LowerArm();
		if(GetPosition() <= 60.0/360.0 * 174.9)
			PullWinch();
		if(mRaiseMotor->IsRevLimitSwitchClosed()) //stop if the arm is lowered all the way
		{
			ArmRLStop();
			StopWinch();
			mArmStep = kDone;
		}
	}

	return mArmStep;
}

void Arm::ResetHookProcess()
{
	mArmStep = kRaisingToVertical;
}

float Arm::CheckDirectionChange(float NewSpeed)
{
	// update position accumulator if changing direction
	// encoder doesn't know the direction so we have to remember the direction for it
	if ((mSpeedPrevious < 0 && NewSpeed >= 0) || (mSpeedPrevious >= 0 && NewSpeed < 0))
	{
		mPosition = GetPosition(); // changing directions so save what we have
		mCounter->Reset(); // and start counting in the new direction
		mSpeedPrevious = NewSpeed; // return input speed for ease of use (may include it in the Set() argument => Set(CheckDirectionChange(speed)))
	}
	return NewSpeed;
}

int Arm::GetPosition()
{
	// position from previous direction change plus what's been accumulated so far in this direction
	if (mSpeedPrevious >= 0)
		return mPosition + mCounter->Get(); // been going forward so add counter

	return mPosition - mCounter->Get(); // been going backward so subtract counter
}

void Arm::ExtendArm()
{
	mExtendMotor->Set(1.0);
}

void Arm::RetractArm()
{
	mExtendMotor->Set(-1.0);
}

void Arm::PullWinch()
{
	mLeftWinchMotor->Set(0.95);
	mRightWinchMotor->Set(0.95);
}

void Arm::ReleaseWinch()
{
	mLeftWinchMotor->Set(-0.4);
	mRightWinchMotor->Set(-0.4);
}

bool Arm::ReadArmPivot()
{
	return mRaiseMotor->Get();
}

bool Arm::ReadArmExtend()
{
	return mExtendMotor->Get();
}

void Arm::ArmRLStop()
{
	mRaiseMotor->Set(CheckDirectionChange(0.0));
}

void Arm::ArmERStop()
{
	mExtendMotor->Set(0.0);
}

void Arm::StopWinch()
{
	mLeftWinchMotor->Set(0.0);
	mRightWinchMotor->Set(0.0);
}

void Arm::Test(std::shared_ptr<XboxJoystick> xbox)
{
	float leftY;
	bool printMenu = true;

	while(xbox->GetX() == false)
	{
		if(printMenu == true)
		{
			printf("Press X to Exit\n");
			printf("Press A and Left Y to pivot arm up and down\n");
			printf("Press B and Left Y to extend and retract the arm\n");
			printf("Press Y and Left Y to Wind and Unwind Winch\n");
			printMenu = false;
		}

		while(xbox->GetA())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetA())
			{
				mRaiseMotor->Set(leftY);
				printf("Pivot arm speed: %f, Pivot Arm Encoder: %d\n",leftY, GetPosition());
			}
			else
				mRaiseMotor->Set(0);
		}
		mRaiseMotor->Set(0);

		while(xbox->GetB())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetB())
			{
				mExtendMotor->Set(leftY);
				printf("Extend arm speed = %f\n",leftY);
			}
			else
			{
				mExtendMotor->Set(0);
			}
		}
		mExtendMotor->Set(0);

		while(xbox->GetY())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetY())
			{
				mLeftWinchMotor->Set(leftY);
				mRightWinchMotor->Set(leftY);
				printf("Winch speed = %f\n",leftY);
			}
			else
			{
				mLeftWinchMotor->Set(0);
				mRightWinchMotor->Set(0);
			}
		}
		mLeftWinchMotor->Set(0);
		mRightWinchMotor->Set(0);
		Wait(0.02);
	}
}

