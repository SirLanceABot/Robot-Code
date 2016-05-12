#include "Headers.h"

#define DROP_SIX
//#define MECANUM

class Robot : public SampleRobot
{
public:
	Robot();
	~Robot();
	void Autonomous();
	void OperatorControl();
	void Test();
	void ResetForAuto(bool &);
	void RobotInit(){}
	void Disabled()
	{
		mRobot->ArcadeDrive(0.0,0.0,true);
		mArmMotor->Disable();
		mElevatorMotor->Disable();
		printf("The robot has been disabled.\n");
	}
	bool TestSensor();

private:
	RobotDrive* mRobot;

#ifdef DROP_SIX
	Talon* mRightMotor;
	Talon* mLeftMotor;

#else
	Talon* mFrontRight;
	Talon* mFrontLeft;
	Talon* mRearRight;
	Talon* mRearLeft;
#endif

	LHSJoystick* mXbox;
	Joystick* mSwitchboard;
	Switchboard* mLaunchpad;
	Gyro* mGyro;
	AnalogInput* mSonar;
	AnalogInput* mCalibrateSonar;
	AnalogInput* mIRSensor;

	Encoder* mLeftEncoder;
	Encoder* mRightEncoder;
	Encoder* mArmEncoder;
	AnalogPotentiometer* mElevatorStringPot;

	DigitalInput* mHallEffectOut;
	DigitalInput* mHallEffectIn;
	DigitalInput* mElevBottomSwitch;
	DigitalInput* mElevTopSwitch;

	DigitalInput* mDialOnes;
	DigitalInput* mDialTwos;
	DigitalInput* mDialFours;
	DigitalInput* mDialEights;

	//Class objects
	GyroDrive* mGyroDriveClass;
	Drive* mDriveClass;
	LHSVision* mLHSVisionClass;
	Elevator* mElevatorClass;
	Arm* mArmClass;

	Talon* mArmMotor;
	Talon* mElevatorMotor;

	Timer* mTimer;

	int mode = 1;
};

Robot::Robot()
{
#ifdef DROP_SIX
	mRightMotor = new Talon(RIGHT_DRIVE_TALON);
	mLeftMotor = new Talon(LEFT_DRIVE_TALON);
	mRobot = new RobotDrive(mLeftMotor, mRightMotor);
	mRobot->SetInvertedMotor(RobotDrive::kFrontRightMotor, true);
#else
	mFrontRight = new Talon(FRONT_RIGHT_TALON);
	mFrontLeft = new Talon(FRONT_LEFT_TALON);
	mRearRight = new Talon(REAR_RIGHT_TALON);
	mRearLeft = new Talon(REAR_LEFT_TALON);
	mRobot = new RobotDrive(mFrontLeft, mRearLeft, mFrontRight, mRearRight);
	mRobot->SetInvertedMotor(RobotDrive::kFrontLeftMotor,true);
	mRobot->SetInvertedMotor(RobotDrive::kRearLeftMotor,true);
#endif

	mArmMotor = new Talon(ARM_TALON);
	mElevatorMotor = new Talon(ELEVATOR_TALON);

	mXbox = new LHSJoystick(XBOX_CONTROLLER);
	mSwitchboard = new Joystick(SWITCHBOARD_CONTROLLER);

	mGyro = new Gyro(GYRO_ANALOG_PORT);
	mGyro->InitGyro();
	mGyro->SetSensitivity(Gyro::kDefaultVoltsPerDegreePerSecond * SENSITIVITY_ADJUSTMENT);

	mSonar = new AnalogInput(SONAR_ANALOG_PORT);
	mCalibrateSonar = new AnalogInput(CALIBRATE_SONAR_PORT);
	mIRSensor = new AnalogInput(IR_ANALOG_PORT);
	mHallEffectOut = new DigitalInput(HALL_EFFECT_OUT);
	mHallEffectIn = new DigitalInput(HALL_EFFECT_IN);
	mElevBottomSwitch = new DigitalInput(ELEV_BOTTOM_LIMIT_SWITCH_PORT);
	mElevTopSwitch = new DigitalInput(ELEV_TOP_LIMIT_SWITCH_PORT);

	mDialOnes = new DigitalInput(SIXTEEN_POINT_SWITCH_ONES);
	mDialTwos = new DigitalInput(SIXTEEN_POINT_SWITCH_TWOS);
	mDialFours = new DigitalInput(SIXTEEN_POINT_SWITCH_FOURS);
	mDialEights = new DigitalInput(SIXTEEN_POINT_SWITCH_EIGHTS);

#ifdef DROP_SIX
	mLeftEncoder = new Encoder(LEFT_ENCODER_A, LEFT_ENCODER_B, false, Encoder::k4X);
	mRightEncoder = new Encoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B, true, Encoder::k4X);
#endif
	mArmEncoder = new Encoder(ARM_ENCODER_A,ARM_ENCODER_B, false, Encoder::k4X);
	mElevatorStringPot = new AnalogPotentiometer(STRING_POT_PORT, STRING_POT_UNITS, STRING_POT_OFFSET);

#ifdef DROP_SIX
	mGyroDriveClass = new GyroDrive(mGyro, mLeftMotor, mRightMotor, mXbox, mRobot, mDialOnes, mDialTwos, mDialFours, mDialEights);
	mDriveClass = new Drive(mRightMotor, mLeftMotor, mRobot, mXbox, mSonar, mCalibrateSonar, mLeftEncoder, mRightEncoder, mIRSensor);
#endif
	mLHSVisionClass = new LHSVision(mRobot, mXbox);
	mElevatorClass = new Elevator(mXbox, mElevatorStringPot, mElevatorMotor, mDriveClass, mIRSensor, mElevTopSwitch, mElevBottomSwitch);
	mArmClass = new Arm(mArmEncoder, mXbox, mArmMotor, mHallEffectOut, mHallEffectIn);
	mLaunchpad = new Switchboard(mSwitchboard);

	mRobot->SetExpiration(0.1);

	mTimer = new Timer();
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
}

Robot::~Robot()
{
	delete mTimer;
	mTimer = NULL;

	delete mArmClass;
	mArmClass = NULL;
	delete mElevatorClass;
	mElevatorClass = NULL;
	delete mLHSVisionClass;
	mLHSVisionClass = NULL;
	delete mDriveClass;
	mDriveClass = NULL;
	delete mGyroDriveClass;
	mGyroDriveClass = NULL;

	delete mElevatorStringPot;
	mElevatorStringPot = NULL;
	delete mArmEncoder;
	mArmEncoder = NULL;
	delete mRightEncoder;
	mRightEncoder = NULL;
	delete mLeftEncoder;
	mLeftEncoder = NULL;

	delete mCalibrateSonar;
	mCalibrateSonar = NULL;
	delete mSonar;
	mSonar = NULL;

	delete mGyro;
	mGyro = NULL;
	delete mXbox;
	mXbox = NULL;

	delete mElevatorMotor;
	mElevatorMotor = NULL;
	delete mArmMotor;
	mArmMotor = NULL;

	delete mRobot;
	mRobot = NULL;

#ifdef DROP_SIX
	delete mLeftMotor;
	mLeftMotor = NULL;
	delete mRightMotor;
	mRightMotor = NULL;
#else
	delete mFrontLeft;
	mFrontLeft = NULL;
	delete mFrontRight;
	mFrontRight = NULL;
	delete mRearLeft;
	mRearLeft = NULL;
	delete mRearRight;
	mRearRight = NULL;
#endif
}

void Robot::ResetForAuto(bool &isDoneDriving)
{
	Wait(0.2);
	mLeftEncoder->Reset();
	mRightEncoder->Reset();
	isDoneDriving = false;
}

void Robot::Autonomous()
{
	printf("Start Autonomous");
	mRobot->SetSafetyEnabled(false);
	mLeftEncoder->Reset();
	mRightEncoder->Reset();
	mGyro->Reset();
	bool isDoneDriving = false;
	int autoType;// = 3;
	//	double autoTypeD;
	//	autoTypeD = DBGSLIDER(0); //slider on dashboard
	//	printf("autotypeD: %f\n\n", autoTypeD);
	//	autoType = (int)autoTypeD;

	autoType = mLaunchpad->GetCounter();

	switch(autoType)
	{
	case 0: //drive to auto zone
		printf("Case 0\n");
		while(IsAutonomous() && (isDoneDriving == false))
		{
			isDoneDriving = mDriveClass->DriveDistWithEncoder(38,-1); //three feet forward, undecided
		}
		break;

	case 1: //pick up tote and drive to auto zone
		printf("Case 1\n");
		//Pick up tote
		while(IsAutonomous() && (isDoneDriving == false))
		{
			isDoneDriving = mElevatorClass->ApproachTote();
		}
		ResetForAuto(isDoneDriving);
		while(IsAutonomous() && (isDoneDriving == false))
		{
			isDoneDriving = mElevatorClass->PickUpTote();
		}
		ResetForAuto(isDoneDriving);
		//drive to auto zone
		while(IsAutonomous() && (isDoneDriving == false))
		{
			printf("Backward\n");
			isDoneDriving = mDriveClass->DriveDistWithEncoder(91,1); //7.6 feet backward, undecided
		}
		//printf("Left Encoder: %d", mLeftEncoder->GetRaw());
		break;
	case 2: //pick up container and drive to auto zone
		printf("Case 2\n");
		//pick up container
//		while(IsAutonomous() && (isDoneDriving == false))
//		{
//			isDoneDriving = mArmClass->PickUpContainer();
//		}
		while(mElevTopSwitch->Get() == true)
		{
			mElevatorMotor->Set(-.8);
		}
		mElevatorMotor->Set(0.0);
		ResetForAuto(isDoneDriving);
		//drive to auto zone
		while(IsAutonomous() && (isDoneDriving == false))
		{
			printf("Forward\n");
			isDoneDriving = mDriveClass->DriveDistWithEncoder(91,1); //7.6 feet forward, undecided
		}
		//printf("Left Encoder: %d", mLeftEncoder->GetRaw());
		break;
	case 3:
		printf("Case 3\n");
		//		//pick up container
		//		//		Wait(2.0);
		//		while(IsAutonomous() && (isDoneDriving == false))
		//		{
		//			isDoneDriving = mArmClass->PickUpContainer();
		//		}
		//		ResetForAuto(isDoneDriving);
		//		//spin 180
		while(IsAutonomous() && (isDoneDriving == false))
		{
			//isDoneDriving = mDriveClass->TurnDistWithEncoder(8.0,-1); //37in = 180 degrees
			//isDoneDriving = mDriveClass->TurnAngleWithEncoder(180.0,-1); //angle, 1 = clockwise
			isDoneDriving = mGyroDriveClass->TurnAngle(180.0, 1, 0); //angle, forceTurn, driveAfter
			Wait(0.02);
		}
		//Jog Right
		ResetForAuto(isDoneDriving);

		while(IsAutonomous() && (isDoneDriving == false))
		{
			//isDoneDriving = mDriveClass->TurnDistWithEncoder(8.0,-1); //37in = 180 degrees
			//isDoneDriving = mDriveClass->TurnAngleWithEncoder(45.0,-1);
			isDoneDriving = mGyroDriveClass->TurnAngle(225.0, 1, 0); //angle, forceTurn, driveAfter (135.0 for jog right)
			Wait(0.02);
		}
		ResetForAuto(isDoneDriving);

		while(IsAutonomous() && (isDoneDriving == false)) //drive 8.5 inches backward
		{
			isDoneDriving = mDriveClass->DriveDistWithEncoder(12.0,-1); // 8.5?
			Wait(0.02);
		}
		ResetForAuto(isDoneDriving);

		while(IsAutonomous() && (isDoneDriving == false))
		{
			//isDoneDriving = mDriveClass->TurnDistWithEncoder(9,1); //37in = 180 degrees
			//isDoneDriving = mDriveClass->TurnAngleWithEncoder(45.0,1);
			isDoneDriving = mGyroDriveClass->TurnAngle(180.0, 1, 0); //angle, forceTurn, driveAfter
			Wait(0.02);
		}
		ResetForAuto(isDoneDriving);

		while(IsAutonomous() && (isDoneDriving == false)) //drive 5 feet forward
		{
			isDoneDriving = mDriveClass->DriveDistWithEncoder(60.0,1); //not yet determined
			Wait(0.02);
		}
		ResetForAuto(isDoneDriving);
		Wait(2.0);
		//pick up tote
		//		while(IsAutonomous() && (isDoneDriving == false))
		//		{
		//			isDoneDriving = mElevatorClass->ApproachTote();
		//		}
		//		ResetForAuto(isDoneDriving);
		//		while(IsAutonomous() && (isDoneDriving == false))
		//		{
		//			isDoneDriving = mElevatorClass->PickUpTote();
		//		}
		//		ResetForAuto(isDoneDriving);
		//drive to auto zone
		while(IsAutonomous() && (isDoneDriving == false)) //drive 6 feet backward
		{
			isDoneDriving = mDriveClass->DriveDistWithEncoder(91.0,-1); //7.6 feet
			Wait(0.02);
		}
		break;
	default:
		printf("Type not found.\n");
		break;

		//Adjust and Move to Tote
		// 	mTimer->Start();
		//	while(IsAutonomous() && (mTimer->Get() <= 5))
		//	{
		//		mLHSVisionClass->UseCameraForTape();
		//		Wait(0.02);
		//	}
		//	mTimer->Stop();
		//	mTimer->Reset();
	}
	printf("End Autonomous\n");
}

void Robot::OperatorControl()
{
	mRobot->SetSafetyEnabled(false);
	bool isDoneApproaching = false;

	while(IsOperatorControl())
	{
		//mArmClass->UpdateArm();
		mElevatorClass->UpdateElevator();
		//mElevatorMotor->Set(-mXbox->GetRawAxis(XBOX_RIGHT_Y_AXIS));
		mRobot->ArcadeDrive(mXbox->GetRawAxis(XBOX_LEFT_Y_AXIS, DRIVE_MODE) * -0.6, -0.6 * mXbox->GetRawAxis(XBOX_LEFT_X_AXIS, DRIVE_MODE), true);

		if(mXbox->GetRawButton(XBOX_START_BUTTON) == true)
		{
			int isDoneLifting = false;
			while((IsOperatorControl() && (isDoneLifting == false)) && mXbox->GetRawButton(XBOX_BACK_BUTTON) == false)
			{
				isDoneLifting = mElevatorClass->ApproachTote();
				Wait(0.02);
			}
			isDoneLifting = false;
			while((IsOperatorControl() && (isDoneLifting == false)) && mXbox->GetRawButton(XBOX_BACK_BUTTON) == false)
			{
				isDoneLifting = mElevatorClass->PickUpTote();
				Wait(0.02);
			}
		}

		if(mXbox->GetRawButton(XBOX_Y_BUTTON) == true)
			mElevatorClass->AddToteToStack();

		if(mXbox->GetRawButton(XBOX_X_BUTTON) == true)
		{
			while(isDoneApproaching == false)
			{
				isDoneApproaching = mDriveClass->DriveUntilClose();
			}
			isDoneApproaching = false;
		}

		printf("String Pot: %f\n", mElevatorStringPot->Get());

		Wait(0.01);
	}
}

void Robot::Test() //by Michael
{
	mRobot->SetSafetyEnabled(false);
	int choice = 0;
	bool isDoneTesting = false;
	std::string sChoice = "0";
	DBSTR(0,"0");
	DBSTR(5,"1. Drive Test");
	DBSTR(6,"2. Vision Test");
	DBSTR(7,"3. Arm Test  4. Elev Test");
	DBSTR(8,"5. Sensor Test");
	DBSTR(9,"6. Switchboard Test");
	while(IsTest())
	{
		//		for(int x = 0; x < 25; x++)
		//			printf("\n");
		//		printf("1. Drive Test\n2. Vision Test\n3. Arm Test\n4. Elevator Test\n");
		//		printf("Enter the value of what you wish to test on the dashboard.\n");
		//mDriveClass->GetIRDistance();

		DBSTR(0,"0");
		while(sChoice == "0")
		{
			sChoice = DBGSTR(0);
		}
		if(sChoice == "1")
			choice = 1;
		else if(sChoice == "2")
			choice = 2;
		else if(sChoice == "3")
			choice = 3;
		else if(sChoice == "4")
			choice = 4;
		else if(sChoice == "5")
			choice = 5;
		else if(sChoice == "6")
			choice = 6;
		else
			choice = -1;

		isDoneTesting = false;

		switch(choice)
		{
		case 1:
			printf("***Test Drive***\n");
			printf("Use Right Stick to test motors\n");
			printf("Press A Button to switch sides\n");
			printf("Press X Button to exit\n\n");
			while(isDoneTesting == false)
				isDoneTesting = mDriveClass->TestDrive();
			Wait(1.0);
			break;
		case 2:
			DBSTR(3,"LED 0, isTote");
			DBSTR(4, "LED 1, isTarget");
			DBSTR(0,"Raw Image Mode");
			DBSTR(1,"Camera 1");
			printf("***Testing Vision***\n");
			printf("Camera 1: Right bumper activates Tape mode, left bumper activates Tote mode, Y activates raw image\n");
			printf("Default camera 1 and raw image mode\n");
			printf("Press A to cycle cameras\n\n");
			while(isDoneTesting == false)
				isDoneTesting = mLHSVisionClass->TestVision();
			DBSTR(0," ");
			DBSTR(1," ");
			DBSTR(3," ");
			DBSTR(4," ");
			mLHSVisionClass->TurnLightOff();
			Wait(1.0);
			break;
		case 3:
			printf("***ARM TEST***\n");
			printf("POV Up & Down:     Arm Movement\n");
			printf("A Button:          Toggles Override Mode\n");
			printf("Start Button:      Reset Arm Encoder\n");
			printf("Press X to Exit\n");
			while(isDoneTesting == false)
				isDoneTesting = mArmClass->TestArm();
			Wait(1.0);
			break;
		case 4:
			printf("***Elevator Test***\n");
			printf("Right Stick Up: Move Elevator Up\n");
			printf("Right Stick Down: Move Elevator Down\n");
			printf("Press X to exit elevator test\n");
			while(isDoneTesting == false)
			{
				isDoneTesting = mElevatorClass->TestElevator();
				printf("String Pot: %f\n", mElevatorStringPot->Get());
			}
			Wait(1.0);
			break;
		case 5:
			printf("***Sensor Test***\n");
			printf("Press A to increment modes, gyro/IR, then encoders, then switches.");
			printf("Press X to exit sensor test.");
			Wait(1.0);
			while(isDoneTesting == false)
				isDoneTesting = TestSensor();
			Wait(1.0);
			break;
		case 6:
			printf("***Switchboard Test***\n");
			printf("Press the big and small red buttons together to exit the test.");
			while(isDoneTesting == false)
				isDoneTesting = mLaunchpad->TestSwitchboard();
			Wait(1.0);
			break;
		default:
			printf("Your choice does not exist. Please try again.\n");
			Wait(1.0);
			break;
		}
		//printf("\n\n");
		choice = 0;
		sChoice = "0";
	}
}

bool Robot::TestSensor()
{
	if(mXbox->GetRawButton(XBOX_X_BUTTON) == true)
		return true;
	if(mXbox->GetRawButton(XBOX_A_BUTTON) == true)
	{
		while(mXbox->GetRawButton(XBOX_A_BUTTON) == true)
		{}
		mode++;
		if(mode > 3)
			mode = 1;
	}
	if(mode == 1)
		printf("Gyro: %f, IR: %f, String Pot: %f\n", mGyro->GetAngle(), 10.641 * pow(mIRSensor->GetAverageVoltage(), -1.226), mElevatorStringPot->Get());
	if(mode == 2)
		printf("Left Encoder: %d, Right Encoder: %d, Arm Encoder: %d\n", mLeftEncoder->GetRaw(), mRightEncoder->GetRaw(), mArmEncoder->GetRaw());
	if(mode == 3)
		printf("Hook Out Switch: %d, In Switch: %d | Elev Top Switch: %d, Bottom Switch: %d\n", mHallEffectOut->Get(), mHallEffectIn->Get(), mElevTopSwitch->Get(), mElevBottomSwitch->Get());
	return false;
}

START_ROBOT_CLASS(Robot);
