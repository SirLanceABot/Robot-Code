/*Authors: Julien, Ben
 * Date: 2-1-17
 * Revision Hist: Added comments and made the AutoChoice enum
 *
 */

/*
 * TODO Add code to enable and disable all components within the auto selector
 */

#define NUM_TAPS 20

#include "WPILib.h"
#include "XboxJoystick.h"
#include "DriveTrain.h"
#include "Gear.h"
//#include "Shooter.h"
#include "Climber.h"
#include "Constants.h"
#include "AutoSelect4237.h"
#include "Vision.h"
#include "LightRing.h"
#include <String>
#include <vector>

class Robot: public SampleRobot
{
public:
	enum VisionStage {kNoTargetFound, kHalfTarget, kFullTarget, kDriveForward, kRelease, kDone, kNothing}; //No target found, 1 side of a target found, full target found - all in order
	enum ExposureMode {kUp, kDown};
	enum AutoGearChoice {kLeft = 3, kCenter = 1, kRight = 2, kNone = 4};
	enum AutoBoilerChoice {kBoilerLeft = 3, kBoilerRight = 2};

	static constexpr I2C::Port SIX_DOF = I2C::Port::kMXP;
	static constexpr double SENSITIVITY = ITG3200::SENSITIVITY_SCALE_FACTOR;

	Robot();
	void OperatorControl();
	void Operator();
	void Autonomous();
	void AutoDrive();
	void Disabled();
	void RobotInit();
	void VisionMain();
	void Drive(double speed, double distance);
	void SpinCW(double speed, double angle);
	void SpinCCW(double speed, double angle);

	void RunVision();
	void DropGear();
	void CloseGearHolder();

	double GetFilteredSteeringJoystick(double p_steeringJoystick, double speedJoystick, bool highGear);
	void CheckLighting();
	void SystemCheck();

private:
	std::shared_ptr<XboxJoystick> mXbox;
	std::shared_ptr<DriveTrain> mDrive;
	std::shared_ptr<Gear> mGear;
//	std::shared_ptr<Shooter> mShooter;
	std::shared_ptr<Climber> mClimber;
	std::shared_ptr<AutoSelect4237> mAutoSelect4237;
	std::shared_ptr<Vision> mVision;
	std::shared_ptr<Compressor> mCompressor;
	std::shared_ptr<LightRing> mLight;
	DriverStation& mDriverStation;

	VisionStage mVisionStage;
	AutoGearChoice mGearPosition;
	AutoBoilerChoice mBoilerPosition;
	double mHueLow;
	double mHueHigh;
	double mSaturationLow;
	double mSaturationHigh;
	double mLuminanceLow;
	double mLuminanceHigh;
	double mBatteryID;
	bool mTestMode;
	bool mAutoShooting;
	bool mWasAPressed;
	bool mWasXPressed;
	bool mWasLeftBumperPressed;
	bool mWasRightBumperPressed;
	bool mWasRightTriggerPressed;
	bool mWasPOVDownPressed;
	bool mWasPOVUpPressed;
	bool mIsDroppingGear;


	ExposureMode mExposureMode;


	double mStorage[NUM_TAPS]; // circular storage for the raw values used to compute running average; must be NUM_TAPS or larger (larger would be wasted, though)
	int mStorageHead; // location of the current, newest, next value in the circular storage
	double mTapValue[NUM_TAPS]; // computed running averages for each tap from 0 to NUM_TAPS-1
	int mTapRange[2][5] = {{0, 0, 0, 0, 0}, {0, 4, 9, 14, 19}};
	int mDriver = 0;
	LightRing::Voltage mLightIntensity;
	LightRing::Voltage mLightIntensityPrevious;
};

Robot::Robot():
mXbox(std::make_shared<XboxJoystick>(JOYSTICK::PRIMARY_JOYSTICK)),
mDrive(std::make_shared<DriveTrain>(CAN_PORT::LEFT_DRIVE_MASTER, CAN_PORT::LEFT_DRIVE_SLAVE_1, CAN_PORT::LEFT_DRIVE_SLAVE_2,
		CAN_PORT::RIGHT_DRIVE_MASTER, CAN_PORT::RIGHT_DRIVE_SLAVE_1, CAN_PORT::RIGHT_DRIVE_SLAVE_2,
		PNEUMATICS_PORT::LOW_SPEED, PNEUMATICS_PORT::HIGH_SPEED,
		SIX_DOF, SENSITIVITY)),
mGear(std::make_shared<Gear>(PNEUMATICS_PORT::CLOSE_GEAR, PNEUMATICS_PORT::OPEN_GEAR, PNEUMATICS_PORT::KICKER_OUT, PNEUMATICS_PORT::KICKER_IN)),
//mShooter(std::make_shared<Shooter>(CAN_PORT::SHOOTER, ROBORIO_DIGITAL_PORT::SHOOTER_SOLENOID)),
mClimber(std::make_shared<Climber>(CAN_PORT::CLIMBER_MASTER, CAN_PORT::CLIMBER_SLAVE)),
mAutoSelect4237(std::make_shared<AutoSelect4237>()),
mVision(std::make_shared<Vision>(ROBORIO_ANALOG_PORT::DISTANCE_SONAR)),
mCompressor(std::make_shared<Compressor>()),
mLight(std::make_shared<LightRing>(ROBORIO_DIGITAL_PORT::VISION_LIGHT_POWER, ROBORIO_DIGITAL_PORT::LIGHT_FOURS_DIGIT, ROBORIO_DIGITAL_PORT::LIGHT_TWOS_DIGIT, ROBORIO_DIGITAL_PORT::LIGHT_ONES_DIGIT)),
mDriverStation(DriverStation::GetInstance()),
mVisionStage(kNoTargetFound),
mGearPosition(kCenter),
mBoilerPosition(kBoilerLeft),
mHueLow(51.0),
mHueHigh(99.0),
mSaturationLow(198.0),
mSaturationHigh(255.0),
mLuminanceLow(40.0),
mLuminanceHigh(113.0),
mBatteryID(0),
mTestMode(false),
mAutoShooting(true),
mWasAPressed(false),
mWasXPressed(false),
mWasLeftBumperPressed(false),
mWasRightBumperPressed(false),
mWasRightTriggerPressed(false),
mWasPOVDownPressed(false),
mWasPOVUpPressed(false),
mIsDroppingGear(false),
mExposureMode(kDown), // fixme: caution - actually set in RobotInit
mStorageHead(0), // starting position of the circular storage
mLightIntensity(LightRing::Voltage::k135v),
mLightIntensityPrevious(LightRing::Voltage::k135v)
{
	mCompressor->Start();
	mXbox->SetDeadZone(0.1);
}

// Robot Init function
void Robot::RobotInit()
{
	mExposureMode = kDown;
	for (int i = 0; i < NUM_TAPS; i++) mStorage[i] = 0.L; // initialize circular buffer so there is something reasonable to use for the first NUM_TAPS time steps
	mVision->IncreaseExposure();
}

// Calls autonomous code
void Robot::Autonomous()
{
	mVision->StartVisionThread();
	std::cout << "Entered Autonomous\n";
	std::cout << "BatteryID: " << mBatteryID << '\n';
	std::cout << "Battery Voltage: " << mDriverStation.GetBatteryVoltage() << '\n';
	switch(mDriverStation.GetAlliance())
	{
	case DriverStation::kRed:
		std::cout << "Alliance Color: Red" << '\n';
		break;
	case DriverStation::kBlue:
		std::cout << "Alliance Color: Blue" << '\n';
		break;
	case DriverStation::kInvalid:
		std::cout << "Alliance Color: Invalid" << '\n';
		break;
	}
	std::cout << "Driver Station: " << mDriverStation.GetLocation() << '\n';
	std::cout << "Gear Position: " << mAutoSelect4237->GetGearPosition2017() << '\n';
	std::cout << "Boiler Position: " << mAutoSelect4237->GetBoilerPosition2017() << '\n';


	mDrive->SetSafetyEnabled(false);
	mDrive->SetControlModeVoltage();
	mDrive->ShiftDown();
	Wait(0.1);
	AutoDrive();
	mVision->StopVisionThread();
	//mVision->SetGearCamera();
	Wait(0.5);
	mDrive->ShiftUp();
}

// Robot Operator Control, calls operator function
void Robot::OperatorControl()
{
	mVision->StopVisionThread();
	std::cout << "Entered Operator Control\n";
	mLight->TurnLightRingOff(); //turns light ring off
	mDrive->SetSafetyEnabled(false);
	mDrive->SetRampRate(120);

	if (IsOperatorControl() && mTestMode == true)
	{
		mVision->DecreaseExposure();
	}
	while (IsOperatorControl() && mTestMode == true)
	{
		SystemCheck();
		Wait(0.005);
	}

	if (IsOperatorControl() && IsEnabled() && mTestMode == false)
	{
		mVision->DecreaseExposure();
	}
	while(IsOperatorControl() && IsEnabled() && mTestMode == false)
	{
		Operator();
		Wait(0.005);
	}
}

// System Check code
void Robot::SystemCheck()
{
	std::string lightStatus;

	double move = mXbox->GetLeftY();				// variable the value of LeftY is stored to
	double rotate = mXbox->GetRightX();				// variable the value of RightX is stored to
	bool AButton = mXbox->GetA();					// varaible the value of GetA is stored to
	bool YButton = mXbox->GetY();					// variable the value of GetY is stored to
	bool RightBumper = mXbox->GetRightBumper();
	bool LeftBumper = mXbox->GetLeftBumper();		// variable the value of GetLeftBumper is stored to
	double RightTrigger = mXbox->GetRightTrigger();	// variable the value of GetRightTrigger is stored to


	mDrive->ArcadeDrive(move, GetFilteredSteeringJoystick(rotate,  move, mDrive->GetGearPos()));

	if(YButton)
	{
		mLight->TurnLightRingOn();
		mLightIntensity = LightRing::Voltage::k135v; // start auto with brightest light forced
		mLight->SetIntensity(mLightIntensity);
		lightStatus = "On";
	}
	else
	{
		mLight->TurnLightRingOff();
		lightStatus = "Off";
	}

	// SHIFT SPEED
	// Use Left Bumper
	// Requires driver to press and release button before shifting gears again
	if (LeftBumper == true && mWasLeftBumperPressed == false)
	{
		mWasLeftBumperPressed = true;
		mDrive->ShiftSpeed();
	}
	else if (LeftBumper == false && mWasLeftBumperPressed == true)
	{
		mWasLeftBumperPressed = false;
	}

	// DROP GEAR
	// Use A Button
	// Requires driver to press and release button before dropping gear again
	if ((AButton == true && mWasAPressed == false) || mIsDroppingGear)
	{
		mWasAPressed = true;
		mIsDroppingGear = mGear->DropGear();
	}
	else if (AButton == false && mWasAPressed == true)
	{
		mGear->CloseGearHolder();
		mWasAPressed = false;
	}

	// CLIMB ROPE
	// Use Right Trigger
	// when the button is pressed the robot will climb
	if (RightTrigger > 0.25)
	{
		mCompressor->Stop();
		mWasRightTriggerPressed = true;
		mClimber->ClimbRope(RightTrigger);
	}
	else
	{
		mCompressor->Start();
		mWasRightTriggerPressed = false;
		mClimber->StopClimbing();
	}

	if (RightBumper)
	{
		mVision->SetGearCamera();
		while (mXbox->GetRightBumper()) {}
	}

	        std::cout << "Light: " << lightStatus                              ;	//"I like to line up all my semi-colons" -Julien
	      std::cout << "  Gyro: " << (int)mDrive->GetZ()                       ;
	    std::cout << "  LeftEnc: " << mDrive->GetLeftEncoder()                 ;
	  std::cout << "  RightEnc: " << mDrive->GetRightEncoder()                 ;
	std::cout << "  Shifter Speed: " << mDrive->GetShifterPos() << '\n'        ;

}

// Operator Control code
void Robot::Operator()
{
	double move = mXbox->GetLeftY();				// variable the value of LeftY is stored to
	//	double moveRight = mXbox->GetRightY();
	double rotate = mXbox->GetRightX();				// variable the value of RightX is stored to
	bool AButton = mXbox->GetA();					// variable the value of GetA is stored to
	bool XButton = mXbox->GetX();					// variable the value of GetX is stored to
	bool LeftBumper = mXbox->GetLeftBumper();		// variable the value of GetLeftBumper is stored to
	//	bool RightBumper = mXbox->GetRightBumper();		// variable the value of GetRightBumper is stored to
	double RightTrigger = mXbox->GetRightTrigger();	// variable the value of GetRightTrigger is stored to
	double LeftTrigger = mXbox->GetLeftTrigger();
	int POV = mXbox->GetPOV();
	double MatchTime = mDriverStation.GetMatchTime();

	//mDrive->ArcadeDrive(move, rotate);	// calls up arcade drive function
	mDrive->ArcadeDrive(move, GetFilteredSteeringJoystick(rotate,  move, mDrive->GetGearPos()));	// calls up arcade drive function
	//mDrive->Print();

	// RUMBLE
	// Rumbles at 50% power with 40 and 10 seconds left in the match
	// Practice timer counts down
	// Teleop Timer returns -1
	if ((MatchTime >= 40 && MatchTime <= 41) || (MatchTime >= 10 && MatchTime <= 11))
	{
		mXbox->SetRightRumble(1);
	//	std::cout << "Rumble " << MatchTime << '\n';
	}
	else
	{
		mXbox->SetRightRumble(0);
	//	std::cout << "No Rumble " << MatchTime << '\n';
	}

	// SHIFT SPEED
	// Use Left Bumper
	// Requires driver to press and release button before shifting gears again
	if (LeftBumper == true && mWasLeftBumperPressed == false)
	{
		mWasLeftBumperPressed = true;
		mDrive->ShiftSpeed();
	}
	else if (LeftBumper == false && mWasLeftBumperPressed == true)
	{
		mWasLeftBumperPressed = false;
	}

	if (POV == 0 && mWasPOVUpPressed == false)
	{
		mWasPOVUpPressed = true;
		mDrive->ShiftUp();
	}
	else if (POV == -1 && mWasPOVUpPressed == true)
	{
		mWasPOVUpPressed = false;
	}

	if (POV == 180 && mWasPOVDownPressed == false)
	{
		mWasPOVDownPressed = true;
		mDrive->ShiftDown();
	}
	else if (POV == -1 && mWasPOVDownPressed == true)
	{
		mWasPOVDownPressed = false;
	}

	// CLIMB ROPE
	// Use Right Trigger
	// when the button is pressed the robot will climb
	if (RightTrigger > 0.25)
	{
		mCompressor->Stop();
		mWasRightTriggerPressed = true;
		mClimber->ClimbRope(RightTrigger);
		mClimber->Print();
	}
	else
	{
		mCompressor->Start();
		mWasRightTriggerPressed = false;
		mClimber->StopClimbing();
	}
	if(LeftTrigger > 0.5)
	{
		//mClimber->ClimbRope(-0.5);
	}

	// SHOOT BALL
	// Use X Button
	// Requires driver to press and release button before shooting balls again
	if (XButton == true && mWasXPressed == false)
	{
		mWasXPressed = true;
//		mShooter->ShootBall(1.0);
	}
	else if (XButton == false && mWasXPressed == true)
	{
		mWasXPressed = false;
//		mShooter->ShootBall(0.0);
	}

	// DROP GEAR
	// Use A Button
	// Requires driver to press and release button before dropping gear again
	if ((AButton == true && mWasAPressed == false) || mIsDroppingGear)
	{
		mWasAPressed = true;
		mIsDroppingGear = mGear->DropGear();
	}
	else if (AButton == false && mWasAPressed == true)
	{
		mGear->CloseGearHolder();
		mWasAPressed = false;
	}
}

// Drives robot autonomously
void Robot::AutoDrive()
{
	double angle;

	mVision->DecreaseExposure();

	mVision->ResetPiTimer();
	mLight->TurnLightRingOn();
	mLightIntensity = LightRing::Voltage::k135v; // start auto with brightest light forced
	mLight->SetIntensity(mLightIntensity);
	mLightIntensityPrevious = mLightIntensity;

	mDrive->ResetEncoders();
	mDrive->MakeIsDoneDrivingTrue();
	mDrive->ResetGyro();

	switch (mGearPosition)
	{
	case kLeft:
		Drive(AUTONOMOUS::SIDE_SPEED, 70);		//was 86
		SpinCW(AUTONOMOUS::TURN_SPEED, 55);
//		Drive(AUTONOMOUS::SIDE_SPEED, 12);
		Wait(0.2);
		RunVision();

		mDrive->StopDriving();
		mDrive->MakeIsDoneDrivingTrue();
		mDrive->ResetEncoders();

		DropGear();
		Drive(AUTONOMOUS::BACKWARD_SPEED, 12.0);
		CloseGearHolder();

		if(mDrive->IsGyroWorking())
		{
			angle = fabs(mDrive->GetZ());
			SpinCCW(AUTONOMOUS::TURN_SPEED, angle - 5);
		}
		else
		{
			std::cout << "ERROR Gyro not working" << '\n';
			SpinCCW(AUTONOMOUS::TURN_SPEED, 45);
		}

		switch(mBoilerPosition)
		{
		case kBoilerRight:
			Drive(AUTONOMOUS::FINAL_SPEED, 240.0);
			break;
		case kBoilerLeft:
			Drive(AUTONOMOUS::FINAL_SPEED, 100.0);
			SpinCW(AUTONOMOUS::TURN_SPEED, 35);
			Drive(AUTONOMOUS::FINAL_SPEED, 200.0);
			break;
		}

		break;

		case kCenter:
			Drive(AUTONOMOUS::FINAL_SPEED, 40);
			Wait(0.1);
			RunVision();
			DropGear();
			Drive(AUTONOMOUS::BACKWARD_SPEED, 9.0);
			CloseGearHolder();
			break;

		case kRight:
			Drive(AUTONOMOUS::SIDE_SPEED, 80);
			SpinCCW(AUTONOMOUS::TURN_SPEED, 50);
//			Drive(AUTONOMOUS::SIDE_SPEED, 12);
			Wait(0.2);
			RunVision();

			mDrive->StopDriving();
			mDrive->MakeIsDoneDrivingTrue();
			mDrive->ResetEncoders();

			DropGear();
			Drive(AUTONOMOUS::BACKWARD_SPEED, 12.0);
			CloseGearHolder();

			if(mDrive->IsGyroWorking())
			{
				angle = fabs(mDrive->GetZ());
				SpinCW(AUTONOMOUS::TURN_SPEED, angle - 15);
			}
			else
			{
				std::cout << "ERROR Gyro not working" << '\n';
				SpinCW(AUTONOMOUS::TURN_SPEED, 35);
			}

			switch(mBoilerPosition)
			{
			case kBoilerLeft:
				Drive(AUTONOMOUS::FINAL_SPEED, 240.0);
				break;
			case kBoilerRight:
				Drive(AUTONOMOUS::FINAL_SPEED, 100.0);
				SpinCCW(AUTONOMOUS::TURN_SPEED, 35);
				Drive(AUTONOMOUS::FINAL_SPEED, 200.0);
				break;
			}

			break;	//break out of mAutonomous Choice case kRight

		case kNone:
			Drive(AUTONOMOUS::FINAL_SPEED, 86);
			break;

		default:
			std::cout << "Unknown case mAutonomousChoice"  << '\n';
	}
}

// Drive forward at certain speed and distance
void Robot::Drive(double speed, double distance)
{
	std::cout << "Driving" << '\n';
	while(IsAutonomous() && IsEnabled() && !mDrive->NewDriveDistance(speed, distance))
	{
		Wait(0.005);
	}
	mDrive->StopDriving();
	mDrive->MakeIsDoneDrivingTrue();
	mDrive->ResetEncoders();
	std::cout << "Done driving" << '\n';
}


// Runs vision code
void Robot::RunVision()
{
	std::cout << "Starting vision" << '\n';
	if(IsAutonomous() && IsEnabled())
	{
		mVision->StartPiTimer();
		VisionMain();
	}
	std::cout << "Done running vision" << '\n';
}

// Drops gear
void Robot::DropGear()
{
	std::cout << "Dropping gear" << '\n';
	while(IsAutonomous() && IsEnabled() && mGear->DropGear())
	{
		Wait(0.005);
		//mDrive->Print();
	}
	std::cout << "Done dropping gear" << '\n';
}

// Closes gear holder
void Robot::CloseGearHolder()
{
	std::cout << "Closing gear holder" << '\n';
	mGear->CloseGearHolder();
	std::cout << "Done closing gear holder" << '\n';
}
// Spin CW at certain speed and angle
void Robot::SpinCW(double speed, double angle)
{
	std::cout << "Turning" << '\n';
	while(IsAutonomous() && IsEnabled() && !mDrive->NewSpinCW(speed, angle))
	{
		Wait(0.005);
	}
	mDrive->StopDriving();
	mDrive->MakeIsDoneDrivingTrue();
	mDrive->ResetEncoders();
	std::cout << "Done turning" << '\n';
}

// Spin CCW at certain speed and angle
void Robot::SpinCCW(double speed, double angle)
{
	std::cout << "Turning" << '\n';
	while(IsAutonomous() && IsEnabled() && !mDrive->NewSpinCCW(speed, angle))
	{
		Wait(0.005);
	}
	mDrive->StopDriving();
	mDrive->MakeIsDoneDrivingTrue();
	mDrive->ResetEncoders();
	std::cout << "Done turning" << '\n';
}

//Disables autonomous code. NO AUTO
void Robot::Disabled() // FIXME:  Disabled has mucho code!!
{
	mDrive->ResetEncoders();
	mDrive->MakeIsDoneDrivingTrue();
	mDrive->ResetGyro();
	//	std::cout << "Make your autonomous choice (X for left, A for center, B for right)";
	std::cout << "Entered disabled\n";
	mVision->DecreaseExposure();

	while (IsDisabled())
	{

		if (mAutoSelect4237->GetGearPosition2017() == "Left")
		{
			//std::cout << "LEFT" << '\n';
			mGearPosition = kLeft;
		}
		else if (mAutoSelect4237->GetGearPosition2017() == "Center")
		{
			//std::cout << "CENTER" << '\n';
			mGearPosition = kCenter;
		}
		else if (mAutoSelect4237->GetGearPosition2017() == "Right")
		{
			//std::cout << "RIGHT" << '\n';
			mGearPosition = kRight;
		}
		else if (mAutoSelect4237->GetGearPosition2017() == "None")
		{
			//std::cout << "NONE" << '\n';
			mGearPosition = kNone;
		}

		if (mAutoSelect4237->GetBoilerPosition2017() == "Left")
		{
			mBoilerPosition = kBoilerLeft;
		}
		else if(mAutoSelect4237->GetBoilerPosition2017() == "Right")
		{
			mBoilerPosition = kBoilerRight;
		}

		mTestMode = mAutoSelect4237->GetTestMode2017();
		mAutoShooting = mAutoSelect4237->GetAutoShooting2017();
		mBatteryID = mAutoSelect4237->GetBatteryID2017();

		mHueLow = mAutoSelect4237->GetHueLow2017();
		mHueHigh = mAutoSelect4237->GetHueHigh2017();
		mSaturationLow = mAutoSelect4237->GetSaturationLow2017();
		mSaturationHigh = mAutoSelect4237->GetSaturationHigh2017();
		mLuminanceLow = mAutoSelect4237->GetLuminanceLow2017();
		mLuminanceHigh = mAutoSelect4237->GetLuminanceHigh2017();

		Wait(0.005);
	}
}

void Robot::VisionMain()
{
	bool isUpdateGood;
	int numTargets;
	mDrive->StopDriving();
	isUpdateGood = mVision->Update(); //Update all values in vision
	if(isUpdateGood == true)
	{
		numTargets = mVision->GetNumTargets();
	}
	else
	{
		numTargets = 0;
	}

	/*
	 * Determine state of target - 0, 1, or 2 of them
	 */
	if (numTargets == 0)
	{
		std::cout << "[Robot::VisionMain() " << __LINE__ << "] No target found in auto" << '\n';
		mVisionStage = kNoTargetFound;

		while (IsAutonomous() && IsEnabled() && (numTargets <= 0))
		{
			std::cout << "[Robot::VisionMain() " << __LINE__ << "] No targets found - trying again" <<  '\n';
			isUpdateGood = mVision->Update();
			if(isUpdateGood == true)
			{
				numTargets = mVision->GetNumTargets();
			}
			Wait(0.02);
		}
		std::cout << "[Robot::VisionMain() " << __LINE__ << "] Finally found some target - moving on" << '\n';
	}

	if (numTargets == 1)
	{
		std::cout << "[Robot::VisionMain() " << __LINE__ << "] Half target found in auto" << '\n';
//		mVisionStage = kHalfTarget;
		mVisionStage = kFullTarget;
	}

	else
	{
		std::cout << "[Robot::VisionMain() " << __LINE__ << "] Full target found in auto" << '\n';
		mVisionStage = kFullTarget;
	}

	/*
	 * Run code for one strip of tape found
	 */
	while(IsAutonomous() && IsEnabled() && mVisionStage == kHalfTarget)
	{
		//CheckLighting();
		isUpdateGood = mVision->Update();
		if(isUpdateGood == true)
		{
			numTargets = mVision->GetNumTargets();

			if (numTargets == 0) //Check if no targets found
			{
				std::cout << "[Robot::VisionMain() " << __LINE__ << "] Nothing found in half target, stopping" << '\n';
				mDrive->StopDriving(); // stop and hang in this loop ; better to scan back the other way to find contours
			}

			if (numTargets == 2) //Check if there are two targets found
			{
				mDrive->StopDriving(); //Stop robot
				mVisionStage = kFullTarget; //Advance to next stage
			}

			else // try to turn to find other contour
			{
				mDrive->ArcadeDrive(0.0, mVision->GetTurnSpeedUsingCenter(), false); // turn slowly to find other contour
			}
		}
		else
		{
			mDrive->StopDriving();
		}

		Wait(0.02);
	}

	/*
	 * Run code for aligning with two strips of tape
	 */
	while(IsAutonomous() && IsEnabled() && mVisionStage == kFullTarget)
	{
		//CheckLighting();
		isUpdateGood = mVision->Update();
		if(isUpdateGood == true)
		{
			std::cout << '\n' << "[Robot::VisionMain() " << __LINE__ << "] Aligning using full target" << '\n';

			if (mVision->IsAlignedWithTarget()) //If robot is aligned with target
			{
				std::cout << "[Robot::VisionMain() " << __LINE__ << "] Changing to drive forward state" << '\n';
				mDrive->StopDriving();
				mVisionStage = kDriveForward; //Change stage to DriveForward
			}
			else
			{
				std::cout << "[Robot::VisionMain() " << __LINE__ << "] Turning to center on target" << '\n';
				mDrive->ArcadeDrive(0.0, mVision->GetTurnSpeed(), false);
			}
		}
		else
		{
			mDrive->StopDriving();
		}

		Wait(0.02);
	}

	/*
	 *  Drive forward while maintaining alignment
	 */
	while(IsAutonomous() && IsEnabled() && mVisionStage == kDriveForward) //While no large blob found, do stuff in the loop
	{
		//CheckLighting();
		isUpdateGood = mVision->Update();
		if(isUpdateGood == true)
		{
			if (mVision->IsCloseToTargetWithVision())
			{
				mDrive->StopDriving();
				mVisionStage = kDone;
			}
			else
			{
				std::cout << "[Robot::VisionMain() " << __LINE__ << "] Driving forward" << '\n';
				mDrive->ArcadeDrive(AUTONOMOUS::VISION_SPEED, mVision->GetTurnSpeed(), false);
			}
		}
		else
		{
			mDrive->StopDriving();
		}
		Wait(0.02);
	}

	if (mVisionStage == kDone) //Done
	{
		std::cout << "[Robot::VisionMain() " << __LINE__ << "] Done with vision - stopping" << '\n';
	}

	else
	{
		std::cout << "[Robot::VisionMain() " << __LINE__ << "] Not a good mVisionStage but stopping anyway" << mVisionStage << '\n';
	}

	mDrive->StopDriving();
}

void Robot::CheckLighting()
{
	// control the light ring; if near (big height), reduce power; if far or some problem, increase power

	if (mVision->GetLargestHeight() > 40) // trick!!  area is really height until this is refactored
		mLightIntensity = mLight->LightRing::Voltage::k084v;
	else
		mLightIntensity = mLight->LightRing::Voltage::k135v;

	if (mLightIntensity != mLightIntensityPrevious) // update only if changed, since not sure how fast light controller can respond
	{
		mLight->SetIntensity(mLightIntensity);
		std::cout << "[Autonomous] Changed light intensity to " << mLightIntensity << "\n";
	}

	mLightIntensityPrevious = mLightIntensity;
}

double Robot::GetFilteredSteeringJoystick(double p_steeringJoystick, double speedJoystick, bool highGear)
{
	// Computation of moving average FIR filter

	double steeringJoystick = p_steeringJoystick; // default value if filtering is not needed

	// save raw value in the circular storage of the joystick value
	mStorage[mStorageHead] = p_steeringJoystick;

	// compute new values for all taps
	double runningTotal = 0.L; // clear the runningTotal for next summation
	int i = mStorageHead; // summing starts at tap 0 so start summing the storage buffer at the current value
	for(int tapIndex = 0; tapIndex < NUM_TAPS; tapIndex++) // process each tap
	{
		runningTotal += mStorage[i]; // retrieve from circular storage and sum the stored values
		mTapValue[tapIndex] = runningTotal / (double)(tapIndex+1); // compute the new tap value (average) as we go through the storage
		i--; // storage location of the next oldest value to sum
		if (i < 0) i = NUM_TAPS-1; // check for gone around the circle
	}
	// End Computation of moving average FIR filter

	// use the new tap values
	//		for (int tapIndex = 0; tapIndex < NUM_TAPS; tapIndex=tapIndex + 1) std::cout << ", " << mTapValue[tapIndex]; std::cout << '\n';

	if(highGear && fabs(speedJoystick) > .5) // use the position of the power train transmission; only deaden stick at high end of high gear
	{
		if(fabs(speedJoystick) > .5 && fabs(speedJoystick) <= .6) steeringJoystick = mTapValue[mTapRange[mDriver][0]];
		else
			if(fabs(speedJoystick) > .6 && fabs(speedJoystick) <= .7) steeringJoystick = mTapValue[mTapRange[mDriver][1]];
			else
				if(fabs(speedJoystick) > .7 && fabs(speedJoystick) <= .8) steeringJoystick = mTapValue[mTapRange[mDriver][2]];
				else
					if(fabs(speedJoystick) > .8 && fabs(speedJoystick) <= .9) steeringJoystick = mTapValue[mTapRange[mDriver][3]];
					else													  steeringJoystick = mTapValue[mTapRange[mDriver][4]];
	}

	// prepare for next iteration

	// next spot to use in circular storage for FIR moving average filters
	mStorageHead++; // prepare storage location of the next value to save
	if (mStorageHead >= NUM_TAPS) mStorageHead = 0; // check for gone around the circle

	return steeringJoystick;
}

START_ROBOT_CLASS(Robot)
