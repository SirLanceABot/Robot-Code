#include "Headers.h"

class CrabDriveMain : public SimpleRobot
{
public:
	enum SteerDirection {kCCW = 1, kCW = -1, kStop = 0};

	CrabDriveMain(void); 
	~CrabDriveMain(void);

	void RobotInit(void);
	void Disabled(void);
	void Autonomous(void);
	void OperatorControl(void);
	void Test(void);
	void ShooterTest(void);
	void SetWheelPosition(void);
	void DriveTrainTest(void);
	void CrabSteerTest(void);
	void ScooperTest(void);
	void LifterTest(void);
	void StopAll(void);
	void TestAll(void);
	void XBoxControl(void);
	void SwitchBoardControl(void);
	void LogitechControl(void);
	void LogitechDriveControl(void);
	void XBoxDriveControl(void);
	void TankDrive(void);
	void SpinRobot(double);
	void TightTurn(double);

private: 
	//LHSVision* mLHSVision;
	CrabDriveSteer* mCrabDriveSteer;
	DriveTrain* mDriveTrain;
	Shooter* mShooter;
	Scooper* mScooper;
	Lifter* mLifter;
	XBoxJoystick* mXBoxJoystick;
	LogitechJoystick* mLogitechJoystick;
	Timer* mVisionTimer;
	DriverStation* mDriverStation;
	//SwitchBoard* mSwitchBoard;


	double mSpeed;
	int mAutonomousMode;
	int mControlType;
};

CrabDriveMain::CrabDriveMain(void)
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
//	mLHSVision = new LHSVision();
	mCrabDriveSteer = new CrabDriveSteer();
	mDriveTrain = new DriveTrain();
	mShooter = new Shooter();
	mScooper = new Scooper();
	mLifter = new Lifter();
	mXBoxJoystick = new XBoxJoystick();
	mLogitechJoystick = new LogitechJoystick();
	mVisionTimer = new Timer();
	mDriverStation = DriverStation::GetInstance();
	//mSwitchBoard = new SwitchBoard();

	mSpeed = 0.0;
	mAutonomousMode = -1;
	mControlType = 1;
}

CrabDriveMain::~CrabDriveMain(void)
{
	//delete mLHSVision;
	//mLHSVision = NULL;
	delete mCrabDriveSteer;
	mCrabDriveSteer = NULL;
	delete mDriveTrain;
	mDriveTrain = NULL;
	delete mShooter;
	mShooter = NULL;
	delete mScooper;
	mScooper = NULL;
	delete mLifter;
	mLifter = NULL;
	delete mXBoxJoystick;
	mXBoxJoystick = NULL;
	delete mLogitechJoystick;
	mLogitechJoystick = NULL;
	delete mVisionTimer;
	mVisionTimer = NULL;
	delete mDriverStation;
	mDriverStation = NULL;
	//delete mSwitchBoard;
	//mSwitchBoard = NULL;
}

void CrabDriveMain::RobotInit(void)
{
	printf("\n\n*** Robot Init Function ***\n\n");

	//	int choice = -1;
	//	int confirm = -2;
	//
	//	do
	//	{
	//		printf("Choose Autonomous Mode\n");
	//		printf("A. Left Side of Field\n");
	//		printf("B. Right Side of Field\n");
	//		while(choice != A_BUTTON && choice != B_BUTTON)
	//		{
	//			choice = mXBoxJoystick->GetXBoxButton();
	//		}
	//
	//		printf("Autonomous Mode = %d\n", choice);
	//		while(mXBoxJoystick->GetXBoxButton() != 0)
	//		{}
	//
	//		printf("Is this correct? Confirm.\n");
	//		printf("A. Left Side of Field\n");
	//		printf("B. Right Side of Field\n");
	//
	//		while(confirm != A_BUTTON && confirm != B_BUTTON)
	//		{
	//			confirm = mXBoxJoystick->GetXBoxButton();
	//		}
	//		while(mXBoxJoystick->GetXBoxButton() != 0)
	//		{}	
	//
	//	}
	//	while(choice != confirm);
	//
	//	mAutonomousMode = choice;
	//	printf("Autonomous Mode = %d\n", mAutonomousMode);
}

void CrabDriveMain::Disabled(void)
{
	printf("\n\n*** Robot Disabled Function ***\n\n");
	mVisionTimer->Stop();
	mVisionTimer->Reset();
	StopAll();
}

void CrabDriveMain::StopAll(void)
{
	mShooter->StopShooter();
	mCrabDriveSteer->StopAllWheels();
	mDriveTrain->StopDrive();
	mScooper->StopScooper();
	mShooter->ResetEncoder();
}

void CrabDriveMain::OperatorControl(void)
{
	printf("Selector 1: Driver Uses Logitech, Co-Driver Uses XBox\n"
			"Selector 2: Driver Uses Logitech, Co-Driver Uses SwitchBoard\n"
			"Selector 3: Driver Uses XBox, Co-Driver Uses SwitchBoard\n"
			"Selector 4: Driver does everything on Logitech\n"
			"Selector 5: Driver does everything on XBox\n");

	while(IsOperatorControl())
	{
		//mControlType = mSwitchBoard->GetSelector();
		if(mControlType == 1) //Driver Uses Logitech, Co-Driver Uses XBox
		{
			LogitechDriveControl();
			XBoxControl();
		}
		else if(mControlType == 2) //Driver Uses Logitech, Co-Driver Uses SwitchBoard
		{
			LogitechDriveControl();
			SwitchBoardControl();

			if(mLogitechJoystick->GetLogitechButton(3)) //??
			{
				mShooter->ResetEncoder();
			}
		}
		else if(mControlType == 3) //Driver Uses XBox, Co-Driver Uses SwitchBoard
		{
			XBoxDriveControl();
			//SwitchBoardControl();

			if(mXBoxJoystick->GetXBoxButton(XBOX_X_BUTTON)) //??
			{
				mShooter->ResetEncoder();
			}
		}
		else if(mControlType == 4) //Driver does everything on Logitech
		{
			LogitechDriveControl();
			LogitechControl();
		}
		else if(mControlType == 5)// Driver controls everything with XBox
		{
			XBoxDriveControl();
			XBoxControl();
		}
	}
}

void CrabDriveMain::XBoxControl(void)
{
	if(mXBoxJoystick->GetXBoxButton(XBOX_Y_BUTTON))
	{
		mShooter->UpdateShooter(Shooter::kShoot);
	}
	else if(mXBoxJoystick->GetXBoxButton(XBOX_X_BUTTON))
	{
		mShooter->UpdateShooter(Shooter::kPass);
	}
	else
	{
		mShooter->UpdateShooter(Shooter::kDoNotShoot);
	}
	if(mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER))
	{
		mShooter->ResetEncoder();
	}

	if(mXBoxJoystick->GetXBoxButton(XBOX_RIGHT_BUMPER))
	{
		mScooper->CalibratePotentiometer();
	}
	if(mXBoxJoystick->GetXBoxButton(XBOX_A_BUTTON))
	{
		mScooper->UpdateScooper(Scooper::kRaise);
	}
	else if(mXBoxJoystick->GetXBoxButton(XBOX_B_BUTTON))
	{
		mScooper->UpdateScooper(Scooper::kLower);
	}
	else
	{
		mScooper->StopScooper();
	}

	if(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
	{
		mLifter->UpdateLifter(Lifter::kRaise);
	}
	if(mXBoxJoystick->GetXBoxButton(XBOX_BACK_BUTTON))
	{
		mLifter->UpdateLifter(Lifter::kLower);

	}
}

void CrabDriveMain::SwitchBoardControl(void)
{/*
	if(mSwitchBoard->GetSwitch(SB_LITTLE_RED_BUTTON))
	{
		mShooter->UpdateShooter(Shooter::kShoot);
	}
	else
	{
		mShooter->UpdateShooter(Shooter::kDoNotShoot);
	}

	if(mSwitchBoard->GetSwitch(SB_JOYSTICK_UP))
	{
		mScooper->UpdateScooper(Scooper::kRaise);
	}
	else if(mSwitchBoard->GetSwitch(SB_JOYSTICK_DOWN))
	{
		mScooper->UpdateScooper(Scooper::kLower);
	}
	else
	{
		mScooper->StopScooper();
	}

	if(mSwitchBoard->GetSwitch(SB_SWITCH_4))
		mLifter->UpdateLifter(Lifter::kRaise);
	if(!mSwitchBoard->GetSwitch(SB_SWITCH_4))
		mLifter->UpdateLifter(Lifter::kLower);
*/}

void CrabDriveMain::LogitechControl(void)
{
	if(mLogitechJoystick->GetLogitechButton(1))
	{
		mShooter->UpdateShooter(Shooter::kShoot);
	}
	else
	{
		mShooter->UpdateShooter(Shooter::kDoNotShoot);
	}
	if(mLogitechJoystick->GetLogitechButton(10))
	{
		mShooter->ResetEncoder();
	}

	if(mLogitechJoystick->GetPOV(LOGITECH_POV_Y) == 1)
	{
		mScooper->UpdateScooper(Scooper::kRaise);
	}
	else if(mLogitechJoystick->GetPOV(LOGITECH_POV_Y) == -1)
	{
		mScooper->UpdateScooper(Scooper::kLower);
	}
	else
	{
		mScooper->StopScooper();
	}
	if(mLogitechJoystick->GetLogitechButton(12))
	{
		mScooper->CalibratePotentiometer();
	}

	if(mLogitechJoystick->GetLogitechButton(6))
		mLifter->UpdateLifter(Lifter::kRaise);
	if(mLogitechJoystick->GetLogitechButton(4))
		mLifter->UpdateLifter(Lifter::kLower);
}

void CrabDriveMain::XBoxDriveControl(void)
{
	double x, y, z, xRight, driveAngle = 0.000, driveSpeed, robotHeading, frontHeading, backHeading; //, spinAngle;
	int leftDirection, rightDirection, rightBumper;
	//mDriveTrain->SetSafetyEnabled(false);

	x = mXBoxJoystick->GetAxis(XBOX_LEFT_X_AXIS);
	y = mXBoxJoystick->GetAxis(XBOX_LEFT_Y_AXIS);
	z = mXBoxJoystick->GetAxis(XBOX_TRIGGER_AXIS);
	xRight = mXBoxJoystick->GetAxis(XBOX_RIGHT_X_AXIS);
	rightBumper = mXBoxJoystick->GetXBoxButton(XBOX_RIGHT_BUMPER);

	frontHeading = mCrabDriveSteer->GetFrontHeading();
	backHeading = mCrabDriveSteer->GetBackHeading();
	robotHeading = mCrabDriveSteer->GetRobotHeading();

	//printf("\n");
	if((fabs(x) > .2 || fabs(y) > .2) && fabs(xRight) < .2 && mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER) != 1) 
	{
		driveAngle = mXBoxJoystick->GetLeftJoystickAngle();
		driveSpeed = mXBoxJoystick->GetLeftJoystickSpeed();
		//printf("Angle: %f Speed: %f\n", driveAngle, driveSpeed);
		mCrabDriveSteer->UpdateSteer(driveAngle);
		rightDirection = mCrabDriveSteer->GetRightDirection();
		leftDirection = mCrabDriveSteer->GetLeftDirection();
		mDriveTrain->UpdateDrive(driveSpeed * leftDirection, driveSpeed * rightDirection);
		//printf("DS: %f\n", driveSpeed);
		//printf("Left: %d Right: %d\n", leftDirection, rightDirection);
	}
	else if(fabs(xRight) > .2 && mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER) == 0)
	{
		SpinRobot(xRight);
	}
	else if(mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER) == 1)
	{
		TankDrive();
	}
	else if(rightBumper == 1)
	{
		mDriveTrain->SonarDrive();
	}
	else
	{
		mDriveTrain->StopDrive();
		mCrabDriveSteer->StopAllWheels();
	}
	printf("Robot Heading: %.3f  Front Encoder: %.3f  Back Encoder: %.3f\n", mCrabDriveSteer->GetRobotHeading(), mCrabDriveSteer->GetFrontHeading(), mCrabDriveSteer->GetBackHeading());
}

void CrabDriveMain::TankDrive(void)
{
	double leftY, rightY, robotHeading, frontHeading, backHeading;
	leftY = mXBoxJoystick->GetAxis(XBOX_LEFT_Y_AXIS);
	rightY = mXBoxJoystick->GetAxis(XBOX_RIGHT_Y_AXIS);
	robotHeading = mCrabDriveSteer->GetRobotHeading();
	frontHeading = mCrabDriveSteer->GetFrontHeading();
	backHeading = mCrabDriveSteer->GetBackHeading();

	if(fabs(rightY) > .1 && fabs(leftY) > .1)
	{
		if((fabs((robotHeading + 90) - frontHeading) < MIN_ANGLE_TOLERANCE && fabs((robotHeading + 90) - backHeading) < MIN_ANGLE_TOLERANCE) || 
				(fabs((robotHeading + 270) - frontHeading) < MIN_ANGLE_TOLERANCE && fabs((robotHeading + 270) - backHeading) < MIN_ANGLE_TOLERANCE))
		{
			mDriveTrain->UpdateDrive(leftY, rightY);
		}
		else
		{
			mDriveTrain->StopDrive();
			mCrabDriveSteer->UpdateSteer(robotHeading + 90);
		}
	}
}

void CrabDriveMain::SpinRobot(double direction)
{

	double frontHeading = mCrabDriveSteer->GetFrontHeading();
	double backHeading = mCrabDriveSteer->GetBackHeading();
	double robotHeading = mCrabDriveSteer->GetRobotHeading();

	if((fabs(mCrabDriveSteer->NormalizeAngle(robotHeading + 90) - frontHeading) < MID_ANGLE_TOLERANCE && fabs(mCrabDriveSteer->NormalizeAngle(robotHeading + 90) - backHeading) < MID_ANGLE_TOLERANCE) || 
			(fabs(mCrabDriveSteer->NormalizeAngle(robotHeading + 270) - frontHeading) < MID_ANGLE_TOLERANCE && fabs(mCrabDriveSteer->NormalizeAngle(robotHeading + 270) - backHeading) < MID_ANGLE_TOLERANCE))
	{
		if(direction > 0)
		{
			mDriveTrain->SpinRobot(1 * mCrabDriveSteer->GetLeftDirection());
		}
		if(direction < 0)
		{
			mDriveTrain->SpinRobot(-1 * mCrabDriveSteer->GetLeftDirection());
		}
	}
	else
	{
		mDriveTrain->StopDrive();
		mCrabDriveSteer->UpdateSteer(robotHeading + 90);
	}
}

void CrabDriveMain::TightTurn(double direction)
{
	int leftDirection, rightDirection;


	if(mCrabDriveSteer->TightTurn(-45))
	{
		leftDirection = mCrabDriveSteer->GetLeftDirection();
		rightDirection = mCrabDriveSteer->GetRightDirection();

		if(direction > 0)
			mDriveTrain->UpdateDrive(.5 * leftDirection, .5 * rightDirection);
		if(direction < 0)
			mDriveTrain->UpdateDrive(-.5 * leftDirection, -.5 * rightDirection);
	}
	else
	{
		mDriveTrain->StopDrive();
	}
	/*else if(direction < 0)
	{
		if(mCrabDriveSteer->TightTurn(45))
		{
			leftDirection = mCrabDriveSteer->GetLeftDirection();
			rightDirection = mCrabDriveSteer->GetRightDirection();

			mDriveTrain->UpdateDrive(.5 * leftDirection, .5 * rightDirection);
		}
		else
		{
			mDriveTrain->StopDrive();
		}
	}
	else
	{
		mDriveTrain->StopDrive();
	}*/
}

void CrabDriveMain::LogitechDriveControl(void) 
{
	double x, y, z, driveAngle = 0.000, driveSpeed, robotHeading, frontHeading, backHeading; //, spinAngle;
	int leftDirection, rightDirection, button5;

	//mDriveTrain->SetSafetyEnabled(false);

	x = mLogitechJoystick->GetAxis(LOGITECH_X_AXIS);
	y = mLogitechJoystick->GetAxis(LOGITECH_Y_AXIS);
	z = mLogitechJoystick->GetAxis(LOGITECH_Z_AXIS);
	button5 = mLogitechJoystick->GetLogitechButton(5);

	frontHeading = mCrabDriveSteer->GetFrontHeading();
	backHeading = mCrabDriveSteer->GetBackHeading();
	robotHeading = mCrabDriveSteer->GetRobotHeading();

	//printf("\n");
	if((fabs(x) > .2 || fabs(y) > .2) && mLogitechJoystick->GetLogitechButton(2) == 0 && mLogitechJoystick->GetLogitechButton(1) == 0)
	{
		driveAngle = mLogitechJoystick->GetJoystickAngle();
		driveSpeed = mLogitechJoystick->GetJoystickSpeedCubed();
		//printf("Angle: %f Speed: %f\n", driveAngle, driveSpeed);
		mCrabDriveSteer->UpdateSteer(driveAngle);
		rightDirection = mCrabDriveSteer->GetRightDirection();
		leftDirection = mCrabDriveSteer->GetLeftDirection();
		mDriveTrain->UpdateDrive(driveSpeed * leftDirection, driveSpeed * rightDirection);
		//printf("DS: %f\n", driveSpeed);
		//printf("Left: %d Right: %d\n", leftDirection, rightDirection);
	}
	else if(fabs(z) > .1 && mLogitechJoystick->GetLogitechButton(2))
	{
		SpinRobot(z);
		mCrabDriveSteer->SetTargetHeading();
	}
	else if(fabs(z) > .1 && mLogitechJoystick->GetLogitechButton(1))
	{
		TightTurn(z);
		mCrabDriveSteer->SetTargetHeading();
	}
	else if(button5 == 1)
	{
		mDriveTrain->SonarDrive();
	}
	else
	{
		mDriveTrain->StopDrive();
		mCrabDriveSteer->StopAllWheels();
	}
	
	if(mLogitechJoystick->GetLogitechButton(10))
	{
		mCrabDriveSteer->ResetGyro();
	}

	printf("Robot Heading: %.3f  Front Encoder: %.3f  Back Encoder: %.3f\n", mCrabDriveSteer->GetRobotHeading(), mCrabDriveSteer->GetFrontHeading(), mCrabDriveSteer->GetBackHeading());
}

void CrabDriveMain::Autonomous(void)
{
	mVisionTimer->Reset();
	mVisionTimer->Start();
	//printf("Autonomous Mode = %d\n", mAutonomousMode);
	mDriveTrain->ResetEncoder();

	mScooper->ResetPotentiometer();

	//mLHSVision->SetCameraLight(true);

	mDriveTrain->Drive(0.6,0.6);

	while(IsAutonomous() && mVisionTimer->Get() < 0.35)
	{
		printf("MatchTime %f  Timer %f\n", mDriverStation->GetMatchTime(), mVisionTimer->Get());
		Wait(0.01);
	}

	//LHSVision::TargetType autoValue = mLHSVision->UpdateTarget();
	//printf("Update Target Returned: %d\n", autoValue);
	//bool hot = mLHSVision->GetHot();

	int value = 1;

	/*if(autoValue == LHSVision::kColdTarget) //assume left
	{
		value = 0;
	}
	else if(autoValue == LHSVision::kHotLeftTarget || mSwitchBoard->GetSwitch(SB_SWITCH_1)) //robot starts on left
	{
		value = 1;
	}
	else if(autoValue == LHSVision::kHotLeftTarget || !mSwitchBoard->GetSwitch(SB_SWITCH_1)) //robot starts on right
	{
		value = 0;
	}
	else if(autoValue == LHSVision::kHotRightTarget || !mSwitchBoard->GetSwitch(SB_SWITCH_1)) //robot starts on right
	{
		value = 1;
	}
	else if(autoValue == LHSVision::kHotRightTarget || mSwitchBoard->GetSwitch(SB_SWITCH_1)) //robot starts on left
	{
		value = 0;
	}
	else if(autoValue == LHSVision::kNoTarget)
	{
		value = 1;
	}*/

	printf("Vision selected case: %d\n", value);

	//	value = 1; //forcing case 1

	bool stillDriving = true;
	double dist = 10.0;

	switch(value) //UpdateDrive will probably be used here, I just wanted more visual functions for now
	{
	case 0: //drive forward, wait until hot, and shoot high goal
		printf("Case 0, drive forward wait until hot\n");

		//Robot must be facing 90 (forward) before starting

		dist = 6.0;
		while(IsAutonomous() && stillDriving) //keep going until 6 feet
		{
			printf("MatchTime %f  Timer %f  ", mDriverStation->GetMatchTime(), mVisionTimer->Get());
			stillDriving = mDriveTrain->DriveForward(dist); //drive forward
			Wait(0.01);
		}

		mDriveTrain->StopDrive();

		printf("Done driving 14 feet");
		while(IsAutonomous() /*&& !hot*/ &&  mVisionTimer->Get() < 5.5)
		{
			printf("MatchTime %f  Timer %f\n", mDriverStation->GetMatchTime(), mVisionTimer->Get());
			//mLHSVision->FindTarget();
			//hot = mLHSVision->GetHot();
			Wait(0.01);
		}

		/*while(IsAutonomous() && !mScooper->GetIsCycleDone())
		{
			mScooper->UpdateScooper(Scooper::kRelease); 
		}*/

		/*if(hot)
			printf("Target is now hot. Take a shot.\n");
		else
			printf("Target is NOT hot. Shoot anyway.\n");*/

		mShooter->UpdateShooter(Shooter::kShoot);
		while(IsAutonomous() && !mShooter->GetIsCycleDone())
		{
			mShooter->UpdateShooter(Shooter::kDoNotShoot);
		}
		mShooter->StopShooter();

		printf("Done shooting\n");
		printf("MatchTime %f  Timer %f\n", mDriverStation->GetMatchTime(), mVisionTimer->Get());
		break;

	case 1: //drive forward and shoot high goal (already hot)
		printf("Case 1, drive forward and shoot already hot goal\n");
		//Robot must be facing 90 (forward) before starting

		dist = 6.0;
		while(IsAutonomous() && stillDriving/* && mVisionTimer->Get() < 5.5*/) //keep going until 6 feet
		{
			printf("MatchTime %f  Timer %f  ", mDriverStation->GetMatchTime(), mVisionTimer->Get());
			stillDriving = mDriveTrain->DriveForward(dist); //drive forward
			Wait(0.01);
		}

		mDriveTrain->StopDrive();
		printf("Done driving 6 feet");
		

		/*while(IsAutonomous() && !mScooper->GetIsCycleDone())
		{
			mScooper->UpdateScooper(Scooper::kRelease); 
		}*/

		//hot = mLHSVision->GetHot();
		/*if(hot)
			printf("Target is still hot\n");
		else
			printf("Target is not hot\n");*/
		
		Wait(.1);

		mShooter->UpdateShooter(Shooter::kShoot);
		while(IsAutonomous() && !mShooter->GetIsCycleDone())
		{
			mShooter->UpdateShooter(Shooter::kDoNotShoot);
		}
		mShooter->StopShooter();

		printf("Done shooting\n");
		printf("MatchTime %f  Timer %f\n", mDriverStation->GetMatchTime(), mVisionTimer->Get());
		break;

	default: //drive forward and don’t shoot
		printf("Case default, drive forward and don’t shoot\n");
		double dist = 10.0;
		//		//Robot must be facing 90 (forward) before starting

		while(IsAutonomous() && stillDriving) //keep going until 6 feet
		{
			stillDriving = mDriveTrain->DriveForward(dist); //drive forward
		}

		mDriveTrain->StopDrive();
		break;
	}
	//mLHSVision->SetCameraLight(false);
}

void CrabDriveMain::TestAll(void)
{
	bool stillDriving = true;
	mCrabDriveSteer->ResetEncoder();

	while(IsOperatorControl())
	{
		//mShooter->TestShooter(.2);
		printf("Shooter Testing, Press Start to move to Drive\nPress A to raise, Press B to lower\n");
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{
			//printf("Hall effect Sensor: %d", mShooter->GetFrontSwitch());
			printf("Shooter Speed: %f  Shooter Distance: %f  ", mDriverStation->GetAnalogIn(1), mDriverStation->GetAnalogIn(2) * 500);
			if(mXBoxJoystick->GetXBoxButton(XBOX_A_BUTTON))
			{
				mShooter->UpdateShooter(Shooter::kShoot);
			}
			else
			{
				mShooter->UpdateShooter(Shooter::kDoNotShoot);
			}
			if(mXBoxJoystick->GetXBoxButton(XBOX_X_BUTTON))
			{
				mShooter->ResetEncoder();
			}
			Wait(0.02);
			//ShooterTest();
			if(mXBoxJoystick->GetXBoxButton(XBOX_A_BUTTON))
			{
				mShooter->UpdateShooter(Shooter::kShoot);
			}
			else if(mXBoxJoystick->GetXBoxButton(XBOX_B_BUTTON))
			{
				mShooter->UpdateShooter(Shooter::kDoNotShoot);
			}
			else
			{
				mShooter->StopShooter();
			}
		}
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{}

		printf("Drive Testing, Press Start to move to Scooper\n");
		while(!mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{
			XBoxDriveControl();
			//printf("Spin Angle: %f  ", mXBoxJoystick->GetRightJoystickAngle() - mCrabDriveSteer->GetRobotHeading());
			//SetWheelPosition();
			if(mXBoxJoystick->GetXBoxButton(XBOX_BACK_BUTTON))
			{
				mDriveTrain->StopDrive();	
				mCrabDriveSteer->StopAllWheels();
				mCrabDriveSteer->UpdateSmartDashboard(mXBoxJoystick->GetLeftJoystickAngle());

				mXBoxJoystick->WaitForX();
			}
			printf("Robot Heading: %f  Right X Axis: %f\n", mCrabDriveSteer->GetRobotHeading(), mXBoxJoystick->GetAxis(XBOX_RIGHT_X_AXIS));
			Wait(.1);
		}
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{}

		printf("DriveForwardTest\n");
		while(!mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{
			if(stillDriving)
			{
				stillDriving = mDriveTrain->DriveForward(14);
			}
			else
			{
				mDriveTrain->StopDrive();
			}
		}
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{}

		printf("Steer Testing\n");
		while(!mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{
			CrabSteerTest();
		}
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{}

		printf("Scooper Testing, Press Start to move to Lifter\n Press A to raise, Press B to lower\n");
		while(!mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{
			if(mXBoxJoystick->GetXBoxButton(XBOX_A_BUTTON))
			{
				mScooper->UpdateScooper(Scooper::kRaise);
			}
			else if(mXBoxJoystick->GetXBoxButton(XBOX_B_BUTTON))
			{
				mScooper->UpdateScooper(Scooper::kLower);
			}
			else
			{
				mScooper->StopScooper();
			}
		}
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{}

		printf("Lifter Testing, Press Start to move to Shooter\nPress X to raise, Press Y to lower\n");
		while(!mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{
			if(mXBoxJoystick->GetXBoxButton(XBOX_X_BUTTON))
			{
				mLifter->UpdateLifter(Lifter::kRaise);
			}
			else if(mXBoxJoystick->GetXBoxButton(XBOX_Y_BUTTON))
			{
				mLifter->UpdateLifter(Lifter::kLower);
			}
		}
		while(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
		{}
	}
	mDriveTrain->UpdateDrive(0.0,0.0);

	mCrabDriveSteer->SetFrontWheels(0.0);
	mCrabDriveSteer->SetBackWheels(0.0);
}

void CrabDriveMain::Test(void)
{
	while(IsTest())
	{
		SetWheelPosition();
//		mDriveTrain->GetEncoder();
		/*
		if(mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER))
		{
			while(mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER))
			{}
			while(mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER) == 0)
			{
				ShooterTest();
				ScooperTest();
				DriveTrainTest();
				CrabSteerTest();
				LifterTest();
			}
			while(mXBoxJoystick->GetXBoxButton(XBOX_LEFT_BUMPER))
			{}
		}
		 */
	}

}

void CrabDriveMain::SetWheelPosition(void)
{
	if(mLogitechJoystick->GetLogitechButton(1))
	{
		mCrabDriveSteer->ResetEncoder();
	}
	else if(mLogitechJoystick->GetLogitechButton(3))
	{
		mCrabDriveSteer->SetFrontWheels(0.4);
	}
	else if(mLogitechJoystick->GetLogitechButton(4))
	{
		mCrabDriveSteer->SetFrontWheels(-0.4);
	}
	else if(mLogitechJoystick->GetLogitechButton(5))
	{
		mCrabDriveSteer->SetBackWheels(0.4);
	}
	else if(mLogitechJoystick->GetLogitechButton(6))
	{
		mCrabDriveSteer->SetBackWheels(-0.4);
	}
	else
	{
		mCrabDriveSteer->SetFrontWheels(0.0);
		mCrabDriveSteer->SetBackWheels(0.0);
	}

}

void CrabDriveMain::ShooterTest(void)
{
	if(mXBoxJoystick->GetXBoxButton(XBOX_Y_BUTTON))
	{
		mShooter->UpdateShooter(Shooter::kManualUp);
	}
	else if(mXBoxJoystick->GetXBoxButton(XBOX_X_BUTTON))
	{
		mShooter->UpdateShooter(Shooter::kManualDown);
	}
	else
	{
		mShooter->StopShooter();
	}
}

void CrabDriveMain::DriveTrainTest(void)
{
	double speed;
	speed = mXBoxJoystick->GetAxis(XBOX_LEFT_Y_AXIS);
	if(fabs(speed) > .1)
	{
		mDriveTrain->UpdateDrive(speed, speed);
	}
	else
	{
		mDriveTrain->UpdateDrive(0, 0);
	}
	printf("Left D: %f RightD: %f\n", mDriveTrain->GetBackDriveDistance(), mDriveTrain->GetFrontDriveDistance());
}

void CrabDriveMain::CrabSteerTest(void)
{
	double driveAngle = 0.0;
	double x = 0.0, y = 0.0;

	x = mXBoxJoystick->GetAxis(XBOX_RIGHT_X_AXIS);
	y = mXBoxJoystick->GetAxis(XBOX_RIGHT_Y_AXIS);

	if(fabs(x) > .2 || fabs(y) > .2)
	{
		driveAngle = mXBoxJoystick->GetRightJoystickAngle();
		mCrabDriveSteer->UpdateSteer(driveAngle);
	}
	else
	{
		mCrabDriveSteer->StopAllWheels();
	}

	printf("Drive Angle: %f  Front Heading: %f  Back Heading: %f\n", driveAngle, mCrabDriveSteer->GetFrontHeading(), mCrabDriveSteer->GetBackHeading());
}

void CrabDriveMain::ScooperTest(void)
{
	if(mXBoxJoystick->GetXBoxButton(XBOX_A_BUTTON))
	{
		mScooper->UpdateScooper(Scooper::kLower);
	}
	else if(mXBoxJoystick->GetXBoxButton(XBOX_B_BUTTON))
	{
		mScooper->UpdateScooper(Scooper::kRaise);
	}
	else
	{
		mScooper->StopScooper();
	}
	if(mXBoxJoystick->GetXBoxButton(XBOX_RIGHT_BUMPER))
	{
		mScooper->CalibratePotentiometer();
	}
	printf("SE GetAverage: %lu\n", mScooper->GetPotentiometerValue());
}

void CrabDriveMain::LifterTest(void)
{
	if(mXBoxJoystick->GetXBoxButton(XBOX_START_BUTTON))
	{
		mLifter->UpdateLifter(Lifter::kRaise);
	}
	if(mXBoxJoystick->GetXBoxButton(XBOX_BACK_BUTTON))
	{
		mLifter->UpdateLifter(Lifter::kLower);
	}
}

START_ROBOT_CLASS(CrabDriveMain);
