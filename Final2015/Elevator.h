#ifndef ELEVATOR_H
#define ELEVATOR_H

//#define STRING_POT
#define NO_STRING_POT

class Elevator
{
public:
	enum ElevatorState {kAutoMovingDown = -1, kStopped, kAutoMovingUp, kManualMove};

	Elevator(LHSJoystick*, Potentiometer*, Talon*, Drive*, AnalogInput*, DigitalInput*, DigitalInput*);
	~Elevator();
	void UpdateElevator();
	void PotentiometerTest();
	bool ApproachTote();
	bool PickUpTote();
	bool TestElevator();
	void ToteLevel(ElevatorState);
	void AddToteToStack();

private:
	LHSJoystick* mXbox;
	Talon* mElevatorMotor;
	Potentiometer* mPot;
	ElevatorState mElevatorState;
	Drive* mDriveClass;
	AnalogInput* mIRSensor;
	DigitalInput* mElevTopSwitch;
	DigitalInput* mElevBottomSwitch;
	int mToteStop;
};

Elevator::Elevator(LHSJoystick* pXbox, Potentiometer* pPot, Talon* pElevatorMotor, Drive* pDriveClass, AnalogInput* pIRSensor, DigitalInput* pElevTopSwitch, DigitalInput* pElevBottomSwitch)
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
	mXbox = pXbox;
	mPot = pPot;
	mElevatorMotor = pElevatorMotor;
	mElevatorState = kStopped;
	mDriveClass = pDriveClass;
	mIRSensor = pIRSensor;
	mElevTopSwitch = pElevTopSwitch;
	mElevBottomSwitch = pElevBottomSwitch;
	mToteStop = 0;
}

Elevator::~Elevator()
{
	delete mElevTopSwitch;
	mElevTopSwitch = NULL;
	delete mElevBottomSwitch;
	mElevBottomSwitch = NULL;
	delete mIRSensor;
	mIRSensor = NULL;
	delete mDriveClass;
	mDriveClass = NULL;
	delete mElevatorMotor;
	mElevatorMotor = NULL;
	delete mPot;
	mPot = NULL;
	delete mXbox;
	mXbox = NULL;
}

void Elevator::PotentiometerTest()
{
	printf ("%f\n", mPot->Get());
}

void Elevator::UpdateElevator()
{
	double rightY = -mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS, 0);
//	double leftTriggerDown = mXbox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS);
//	double rightTriggerUp = mXbox->GetRawAxis(XBOX_RIGHT_TRIGGER_AXIS);
	double toteHeight = mPot->Get();
	bool topSwitch = mElevTopSwitch->Get();
	bool bottomSwitch = mElevBottomSwitch->Get();

	//If Right Y Axis is pressed and elevator is either stopped or moving manually and toteheight is between 0 and 4
	if((rightY >= .2 && topSwitch == true) || (rightY <= -.2 && bottomSwitch == true))
	{
		mElevatorMotor->Set(-rightY);
		mElevatorState = kManualMove;
//		if (rightY >= 0.2 && topSwitch == true) {printf("Moving up | %d, %d\n", topSwitch, __LINE__);}
//		else if (rightY <= -0.2 && bottomSwitch == true) {printf("Moving down | %d, %d\n", bottomSwitch, __LINE__);}
	}
	//If right Y Axis is not pressed or limit switches are pressed, stop the elevator
	else if((fabs(rightY) < 0.2 && mElevatorState == kManualMove) || topSwitch == false || bottomSwitch == false)
	{
		mElevatorMotor->Set(0);
		mElevatorState = kStopped;
//		if (topSwitch == false) {printf(" Stopped | Top Switch: %d, %d\n", topSwitch, __LINE__);}
//		else if (bottomSwitch == false) {printf("Stopped | Bottom Switch: %d, %d\n", bottomSwitch, __LINE__);}
	}
//	//If right trigger is pressed, move up one tote level unless the top switch is pressed
//	else if(rightTriggerUp >= 0.85 && mElevatorState == kStopped && toteHeight < 4 && topSwitch == true)
//	{
//		mElevatorMotor->Set(-0.3);
//		mElevatorState = kAutoMovingUp;
//		mToteStop = (int) (toteHeight + 1);
//	}
//	//If Elevator is auto-moving up and toteheight > totestop and the top switch is not pressed
//	else if(mElevatorState == kAutoMovingUp && toteHeight >= mToteStop && topSwitch == false)
//	{
//		mElevatorMotor->Set(0);
//		mElevatorState = kStopped;
//	}
//	//If left trigger is pressed, move down one tote level unless the bottom switch is pressed
//	else if(leftTriggerDown >= 0.85 && mElevatorState == kStopped && toteHeight > 0 && bottomSwitch == true)
//	{
//		mElevatorMotor->Set(0.3);
//		mElevatorState = kAutoMovingDown;
//		mToteStop = (int) (toteHeight - 1);
//	}
//	//If Elevator is auto-moving down and toteheight < totestop and the bottom switch is not pressed
//	else if(mElevatorState == kAutoMovingDown && toteHeight <= mToteStop && bottomSwitch == false)
//	{
//		mElevatorMotor->Set(0);
//		mElevatorState = kStopped;
//	}
}

bool Elevator::ApproachTote()
{
	double elevatorHeight = mPot->Get();
	bool isDoneApproaching = false;

	if(elevatorHeight > 0.2)
	{
		mElevatorMotor->Set(0.8); //should be faster
		return isDoneApproaching;
	}
	else
	{
		mElevatorMotor->Set(0.0);
		isDoneApproaching = mDriveClass->DriveUntilClose();
	}

	return isDoneApproaching;
}

bool Elevator::PickUpTote()
{
	double elevatorHeight = mPot->Get();
	bool isDoneLifting = false;

	if(elevatorHeight < 1)
	{
		mElevatorMotor->Set(-0.8); //should be faster
	}
	else
	{
		isDoneLifting = true;
		mElevatorMotor->Set(0.0);
	}
	return isDoneLifting;
}

void Elevator::ToteLevel(ElevatorState Direction)
{
	double toteHeight = mPot->Get();
	if (Direction == kAutoMovingUp)
	{
		if(Direction == kAutoMovingUp && toteHeight < 4)
		{
			mElevatorMotor->Set(0.3);
			mToteStop = (int) (toteHeight + 1);
		}
		if(Direction == kAutoMovingUp && toteHeight >= mToteStop)
		{
			mElevatorMotor->Set(0);
			Direction = kStopped;
		}
	}
	else if (Direction == kAutoMovingDown)
	{
		if(Direction == kAutoMovingDown && toteHeight > 0)
		{
			mElevatorMotor->Set(-0.3);
			mToteStop = (int) (toteHeight - 1);
		}
		else if(Direction == kAutoMovingDown && toteHeight <= mToteStop)
		{
			mElevatorMotor->Set(0);
			Direction = kStopped;
		}
	}
}

//void Elevator::AddToteToStack()
//{
//	double toteHeight;
//	bool isDone = false;
//	//toteHeight = mPot->Get();
//	//double startingToteHeight = mPot->Get();
//	//.16 is about two inches down
//	//while (toteHeight > startingToteHeight - 0.16 )      //Move down two inches
//	//{
//	//toteHeight = mPot->Get();
//	mElevatorMotor->Set(0.5);
//	//}
//	Wait(1.0);
//	mElevatorMotor->Set(0);
//	Wait(0.1);
//	mDriveClass->ResetEncoders();
//while(isDone == false)
//{
//	isDone = mDriveClass->DriveDistWithEncoder(4, 1);
//}
//isDone = false;
//	//while (toteHeight > 0.1)
//	//{
//	//toteHeight = mPot->Get();
//	mElevatorMotor->Set(0.75);
//	//}
//	Wait(1.25);
//	mElevatorMotor->Set(0.0);
//	mDriveClass->ResetEncoders();
//	while(isDone == false)
//	{
//		isDone = mDriveClass->DriveDistWithEncoder(4, -1);
//	}
//	//while (mPot->Get() < 2.2)
//	//{
//	//mElevatorMotor->Set(0.5);
//	//}
//	//mElevatorMotor->Set(0);
//}

void Elevator::AddToteToStack()
{
	double toteHeight;
	bool isDone = false;
	toteHeight = mPot->Get();
	double startingToteHeight = mPot->Get();
	//.16 is about two inches down
	while(toteHeight > startingToteHeight - 0.5 && mXbox->GetRawButton(XBOX_BACK_BUTTON) == false)      //Move down two inches
	{
		toteHeight = mPot->Get();
		mElevatorMotor->Set(-0.5);
	}
	mElevatorMotor->Set(0);
	Wait(0.09);
	while(isDone == false)
	{
		isDone = mDriveClass->DriveDistWithEncoder(4, -1);
	}
	isDone = false;
	while(isDone == false && mElevBottomSwitch->Get() == true && mXbox->GetRawButton(XBOX_BACK_BUTTON) == false)
	{
		isDone = mDriveClass->DriveDistWithEncoder(4, 1);
		mElevatorMotor->Set(-0.8);
	}
	isDone = false;
	Wait(.5);
	mElevatorMotor->Set(0);
}

bool Elevator::TestElevator() //by Mark
{
	if(mXbox->GetRawButton(XBOX_X_BUTTON) == true)
		return true;
#ifdef STRING_POT
	float ElevatorTest_Stick = mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS);
	if (fabs(mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS)) > 0.2 && toteHeight <= 4 && toteHeight >= 0)
	{
		mElevatorMotor->Set(ElevatorTest_Stick);
	}
	if (mXbox->GetRawButton(XBOX_X_BUTTON) == 1)
	{
		ElevatorTestDone = true;
	}
	if (mXbox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS) > 0.5)
	{
		while (mXbox->GetRawAxis(XBOX_LEFT_TRIGGER_AXIS) > 0.5)
		{}
		ToteLevel(kAutoMovingDown);
	}
	if (mXbox->GetRawAxis(XBOX_RIGHT_BUMPER) == 1)
	{
		while (mXbox->GetRawAxis(XBOX_RIGHT_BUMPER) == 1)
		{}
		ToteLevel(kAutoMovingUp);
	}
#else
	float ElevatorTest_Stick = -mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS, 0);
	/*if (fabs(mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS)) > 0.2)
	{
		mElevatorMotor->Set(ElevatorTest_Stick);
	}
	else
	{
		mElevatorMotor->Set(0.0);
	}*/
#endif
	return false;
}

#endif
