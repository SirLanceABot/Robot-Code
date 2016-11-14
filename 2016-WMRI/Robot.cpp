/*
 * Robot.cpp
 *
 *  Created on: Feb 7, 2016
 *      Author: Michael Conard
 **/

#include "WPILib.h"
#include "Shooter.h"
#include "Vision.h"
#include "ITG-3200.h"
#include "DriveTrain.h"
#include "XboxJoystick.h"
#include "Arm.h"
#include "AutoSelect4237.h"
//#include "PDP4237.h"
//#include "SmartDashboard4237.h"
#include "Constants.h"
#include <cstring>
#include <cstdlib>
#include <string>

class Robot: public SampleRobot
{
public:
	Robot();				//Constructor
	~Robot();				//Destructor
	void OperatorControl();
	void Autonomous();
	void Test();
	void Disabled();
	void RobotInit();
	int Targeting();

	void AutoLowerShooter(float);
	void AutoRaiseShooter(float);
	void AutoAdjustToGyro(float);
	void AutoDrive(float);
	void AutoRaiseForVision();
	void AutoRaiseForShooting();
	int AutoTargeting();
	void AutoCheval();
	void AutoPortDCullis();
	void AutoLowBar();

	void ResetArrows();
	void RefreshMenu();
	int SelectTestMode();

private:
	std::shared_ptr<XboxJoystick> mStick;		//Xbox controller
	std::shared_ptr<Shooter> mShooter; //Shooter class object
	std::shared_ptr<Vision> mVision; //Vision object
	std::shared_ptr<Arm> mArm; //Arm object
	std::shared_ptr<DigitalOutput> mLight; //Camera light object
	std::shared_ptr<DriveTrain> mRobot; //Drivetrain object
	std::shared_ptr<ITG3200> mGyro;  //Gyroscope object
	std::shared_ptr<Timer> mTimer;
	std::shared_ptr<NetworkTable> mNetworkTable;
	AutoSelect4237* mAutoSelect;
	//std::shared_ptr<SmartDashboard4237> mDashboard;

	const int MenuMax = 7;
	int CurrentArrow = 1;
	char ArrowSlot1 = ' ';
	char ArrowSlot2 = ' ';
	char ArrowSlot3 = ' ';
	char ArrowSlot4 = ' ';
	char ArrowSlot5 = ' ';
	char ArrowSlot6 = ' ';
	char ArrowSlot7 = ' ';

	//AutoSelect4237 variables start with "AS"
	int ASPosition = 6;
	std::string ASObstacle = "None";
	int ASSpeed = 1.0;
	double ASOrientation = 0.0;
	bool ASEnableArm = false;

};

Robot::Robot(): //What's going on with this function
													mStick(std::make_shared<XboxJoystick>(XBOX::DRIVER_PORT)),
													mShooter(std::make_shared<Shooter>(CAN_PORT::SHOOTER_LEFT_WHEEL, CAN_PORT::SHOOTER_RIGHT_WHEEL, CAN_PORT::RAISE_SHOOTER, CAN_PORT::SHOOTER_PUSH)),
													mVision(std::make_shared<Vision>()),
													mArm(std::make_shared<Arm>(CAN_PORT::ARM_PIVOT, CAN_PORT::ARM_EXTEND, CAN_PORT::LEFT_WINCH, CAN_PORT::RIGHT_WINCH, 5)),
													mLight(std::make_shared<DigitalOutput>(DIO_PORT::LIGHT_RING)),												mRobot(std::make_shared<DriveTrain>(CAN_PORT::FRONT_LEFT, CAN_PORT::REAR_LEFT, CAN_PORT::FRONT_RIGHT, CAN_PORT::REAR_RIGHT)),
													mTimer(std::make_shared<Timer>())

{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mAutoSelect = new AutoSelect4237();
	mNetworkTable = NetworkTable::GetTable("Root");
	double initial_angle[] = {0.0,0.0,0.0};
	mGyro = std::make_shared<ITG3200>(I2C_PORT::SIX_DOF, GYRO::ADDRESS, GYRO::SAMPLE_RATE_FILTER, initial_angle, GYRO::SENSITIVITY);
	//	mTimer->Reset();
}

Robot::~Robot()
{

}

void Robot::Disabled()
{
	printf("\nDisabled\n");
	mLight->Set(false);
	while (IsDisabled())
	{
		ASPosition = mAutoSelect->getPosition();
		ASObstacle = mAutoSelect->getObstacle();
		ASSpeed = mAutoSelect->getSpeed();
	}
}

void Robot::RobotInit() //The new Robotinit function
{
	//Camera server
	//CameraServer::GetInstance()->SetQuality(50);
	//CameraServer::GetInstance()->StartAutomaticCapture("cam0");

	printf("Robot Init\n");
	mTimer->Start();
	//This loop is confirmed working
	/*
	while ((mTimer->Get() < 30 && mNetworkTable->GetBoolean("End_Init", true) == false) && !IsAutonomous())
	{
		ASPosition = mNetworkTable->GetNumber("Position", 6);
		ASObstacle = mNetworkTable->GetString("Obstacle", "None");
		Wait(0.1);
	}
	 */
	/*
	printf("Entering autonomous mode selection...\n");
	while (!IsAutonomous() && !IsOperatorControl() && mAutoSelect->endEarly() == false) //This loop needs to go after everything else in RobotInit so they have time to work
	{
		ASPosition = mAutoSelect->getPosition();
		ASObstacle = mAutoSelect->getObstacle();
		ASSpeed = mAutoSelect->getSpeed();
	}
	printf("Done selecting autonomous modes\n\n");
	 */
}

// An old RobotInit function

/*
void Robot::RobotInit()
{
	printf("Enter Init\n");
	mNetworkTable = NetworkTable::GetTable("SmartDashboard");

	// the following lines set the values for autonomous choosing
	int autoMode = -1;
	bool driveToDefense = true;
	bool crossTheDefense = true;
	bool target = true;
	bool shoot = true;
	bool done = false;
	bool done2 = false;

	SmartDashboard::PutNumber("Auto Mode", -1);

	SmartDashboard::PutBoolean("Drive To Defense", 1);
	SmartDashboard::PutBoolean("Cross the Defense", 1);
	SmartDashboard::PutBoolean("Target", 1);
	SmartDashboard::PutBoolean("Shoot", 1);
	SmartDashboard::PutBoolean("Done", 1);
	SmartDashboard::PutBoolean("Done2", 1);

	mDashboard->SetAuto();

	while(done == false || done2 == false)
	{
		mDashboard->ChooseAuto(autoMode, driveToDefense, crossTheDefense, target, shoot, done, done2);
		printf("autoMode %d, driveToDefense %d, crossTheDefense %d, target %d, shoot %d, done %d, done2 %d\n",
			autoMode, driveToDefense, crossTheDefense, target, shoot, done, done2);
		fflush(NULL);
		Wait(0.05);
	}
	printf("\nDone making decisions\n");

		CANTalon::FeedbackDeviceStatus encoderStatus = mMotor->IsSensorPresent(CANTalon::CtreMagEncoder_Absolute);
		switch(encoderStatus)
		{
		case CANTalon::FeedbackStatusPresent:

			autoNameEncoder = "Encoder Found";
			break;

		case CANTalon::FeedbackStatusNotPresent:

			autoNameEncoder = "Encoder Not Found";
			break;

		case CANTalon::FeedbackStatusUnknown:

			autoNameEncoder = "Encoder Unknown";
			break;
		}

			SmartDashboard::PutString(autoNameEncoder, autoNameEncoder);
	printf("Exit Init\n");
}
 */

void Robot::OperatorControl() //Driver controls driving and shooting
{
	bool raisingToShoot = true;
	int doneTargeting = 0;
	//bool isDriving = true;
	int dPad = -1;
	int armProcess = 0;

	mLight->Set(false);

	while(IsOperatorControl() && IsEnabled())
	{
		//Drive function
		mRobot->ArcadeDrive(mStick->GetLeftYAxis(), mStick->GetLeftXAxis() * 1.2); //standard drive

		//Driving controls
		if((mStick->GetY()) /*|| mStick->GetRightYAxis() < -0.2)*/ && mShooter->IsUpperLimit() == false) //If Y button is pressed, raise the shooter
			mShooter->RaiseShooter();
		else if((mStick->GetA()) /*|| mStick->GetRightYAxis() > 0.2)*/ && mShooter->IsLowerLimit() == false) //If A button is pressed, lower the shooter
			mShooter->LowerShooter();
		else                                                                                                  //If neither Y nor A are pressed, make sure the shooter doesn't move
			mShooter->StopShooter();

		//mShooter->TestRaiseShooter();

		if(mShooter->IsUpperLimit() == true && mStick->GetA() == false) //If the shooter has hit the upper limit on the encoder, make sure it doesn't move
		{
			mShooter->StopShooter();
		}
		if(mShooter->IsLowerLimit() == true && mStick->GetY() == false) //If the shooter has hit the lower limit on the encoder, make sure it doesn't move
		{
			mShooter->StopShooter();
		}

		//mShooter->TestPusher(mStick->GetRightYAxis()); //What does this do?

		/*
				if(mStick->GetBack()) //back button
				{
					mLight->Set(true);
					Wait(0.3);
					doneTargeting = 0;
					while(doneTargeting == 0 && mStick->GetLeftStickPress() == false) //left stick press to cancel
						doneTargeting = Targeting(); //line up with the target
					mLight->Set(false);
				}
		 */
/*
		if(mStick->GetRightBumper()) //Raises the shooter to angle for shooting high goals
		{
			raisingToShoot = true;
			while(raisingToShoot && mStick->GetLeftStickPress() == false) //left stick press to cancel
			{
				raisingToShoot = mShooter->RaiseToShootAngle(); //angle shooter to target
				printf("Aiming for Shooting\n");
			}
		}
		*/
		/*
		if(mStick->GetLeftBumper()) //Raises to angle given by vision??
		{
			raisingToShoot = true;
			while(raisingToShoot && mStick->GetLeftStickPress() == false) //Press left stick to cancel
			{
				raisingToShoot = mShooter->RaiseToVisionAngle(); //Angle shooter to target
				printf("Aiming for Vision\n");
			}
		}
		 */
		if(mStick->GetLeftTrigger() >= 0.5) //Pull in ball
		{
			mShooter->GrabBall();
		}
		else if(mStick->GetRightTrigger() >= 1.5) //Jam shot (this is disabled)
		{
			mRobot->ResetDriveEncoder();
			Wait(0.1);

			raisingToShoot = true;
			/*
		 	 isDriving = true;
			 	 while((isDriving) && mStick->GetLeftStickPress() == false)
				{
					isDriving = mRobot->DriveDistance(4750, -.80);	//first parameter is encoder tick number, second speed
					//printf("performing jam shot\n");
					Wait(0.02);
				}
						mRobot->Stop();
			 */
			while((raisingToShoot) && mStick->GetLeftStickPress() == false)
			{
				raisingToShoot = mShooter->RaiseToJamShotAngle(); //angle shooter to target
				printf("Jam shot\n");
				Wait(0.02);
			}
			mShooter->StopShooter();
			//AutoAdjustToGyro(0.0);
			//Wait(0.3);

			//mShooter->ShootBall(-.60);
		}
		else if(mStick->GetX()) //Shoot ball at low goal speed
		{
			mShooter->GrabBall(); //Pull in ball to make sure it isn't touching wheels
			Wait(0.2);
			mRobot->Stop(); //Stop robot so it can't move while shooting

			mLight->Set(true);
			mShooter->ShootBall(-0.63);
			mLight->Set(false);
		}
		else if(mStick->GetB()) //Shoot ball at high goal speed
		{
			mShooter->GrabBall(); //Pull in ball to make sure it isn't touching wheels
			Wait(0.2);
			mRobot->Stop();
			mLight->Set(true); //Turn light on
			mShooter->ShootBall(-0.9);
			mLight->Set(false); //Turn light off
		}
		else //If the right trigger, x, and b are all not pressed, disable the motors
		{
			mShooter->DisableMotors();
		}

		//Arm Code

		dPad = mStick->GetPOV();
		if(dPad != -1 && mStick->GetRightStickPress())
		{
			if(dPad == 90)
				mArm->RaiseArm();
			else if(dPad == 270)
				mArm->LowerArm();

			if(dPad == 0)
				mArm->ExtendArm();
			else if(dPad == 180)
				mArm->RetractArm();
		}
		else
		{
			mArm->ArmRLStop();
			mArm->ArmERStop();
		}

		if(mStick->GetStart())
		{
			while(armProcess != 2 && mStick->GetLeftStickPress() == false && IsEnabled()) //5 is kDone, 2 is kRaisingPastVeritcal
			{
				armProcess = mArm->AttachHookAutonomously();
			}
			mArm->ResetHookProcess();
			mArm->StopWinch();
			mArm->ArmRLStop();
			mArm->ArmERStop();
		}

		if(mStick->GetRightStickPress() == true && mStick->GetRightYAxis() >= 0.75)
		{
			while(mStick->GetRightYAxis() >= 0.75 && IsEnabled())
				mArm->PullWinch();
			mArm->StopWinch();
		}
		else if( mStick->GetRightStickPress() == true && mStick->GetRightYAxis() <= -0.75)
		{
			while(mStick->GetRightYAxis() <= -0.75 && IsEnabled())
				mArm->ReleaseWinch();
			mArm->StopWinch();
		}

		//Display the shooter encoder
		printf("Shooter Encoder: %d\n", mShooter->GetRaiseEncoder());

		Wait(0.02);
	}
}

void Robot::AutoLowerShooter(float time) //Lower shooter for autonomous for the time it's given
{
	mTimer->Start();
	while(mTimer->Get() < time && IsAutonomous())
	{
		mShooter->LowerShooter(); //Lower the shooter
		Wait(0.02);
	}
	mShooter->StopShooter(); //Stop the shooter

	mTimer->Stop();
	mTimer->Reset();
}

void Robot::AutoRaiseShooter(float time) //Raise shooter in autonomous for the time it's given
{
	mTimer->Start();
	while(mTimer->Get() < time && IsAutonomous())
	{
		mShooter->RaiseShooter();
		Wait(0.02);
	}
	mShooter->StopShooter();

	mTimer->Stop();
	mTimer->Reset();
}

void Robot::AutoAdjustToGyro(float angle)  //Adjust
{
	double zAngle;

	while(IsAutonomous()) // adjust to CW ~30 degrees from outerworks
	{
		zAngle = mGyro->GetZ();
		printf("Gyro: %f\n", zAngle);

		if(zAngle < angle - 4.0)
			mRobot->TankDrive(-AUTONOMOUS::ROTATE_SPEED, AUTONOMOUS::ROTATE_SPEED);
		else if(zAngle > angle + 4.0)
			mRobot->TankDrive(AUTONOMOUS::ROTATE_SPEED, -AUTONOMOUS::ROTATE_SPEED);
		else
		{
			mRobot->ArcadeDrive(0.0,0.0);
			break;
		}
		Wait(0.005);
	}
	mRobot->ArcadeDrive(0.0,0.0); //Stop moving
}

void Robot::AutoDrive(float time) //Drive in autonomous for the time it's given
{
	mTimer->Reset();
	mTimer->Start();

	while(mTimer->Get() < time && IsAutonomous()) //Drive until time is up or autonomous ends
	{
		mRobot->TankDrive(0.88, 0.88); //left is 1% weaker
		Wait(0.02);
	}
	mRobot->ArcadeDrive(0.0,0.0);

	mTimer->Stop();
	mTimer->Reset();
}

void Robot::AutoRaiseForVision()
{
	//FIXME: potential problem is shooter is moving when auto expires it might start
	bool raisingForVision = true;

	while(raisingForVision && IsAutonomous())
	{
		raisingForVision = mShooter->RaiseToVisionAngle();
		//printf("Aiming Shooter for Vision\n");
		Wait(0.005);
	}
	mShooter->StopShooter(); //Stop the shooter after the loop runs
}

void Robot::AutoRaiseForShooting()
{
	//FIXME: potential problem is shooter is moving when auto expires it might start

	bool raisingToShoot = true;
	while(raisingToShoot && IsAutonomous())
	{
		raisingToShoot = mShooter->RaiseToShootAngle();
		//printf("Aiming Shooter for Shooting\n");
		Wait(0.005);
	}
	mShooter->StopShooter(); //Stop the shooter after the loop runs
}

int Robot::AutoTargeting()
{
	// loops while vision okay but off-target
	// stops when on-target or there is a vision problem
	// slight transient vision problem will cause a stop in functioning
	// returns the value from Targeting which is:
	// returns 0 if off target - drive motors running
	// returns 1 if on target - drive motors stopped
	// returns 999 if vision problem - motors unchanged
	// note if autonomous ends and the motors are running they may start running ag

	int doneTargeting = 0;
	while(doneTargeting == 0 && IsAutonomous())  //line up with the target
	{
		doneTargeting = Targeting();
		Wait(0.02);
	}
	if(doneTargeting == 1)
		printf("Centered!\n");
	else
		printf("Auto targeting error or incomplete\n");

	return doneTargeting;
}

void Robot::AutoCheval()
{
	bool isDriving = true;
	mRobot->ResetDriveEncoder();
	while(isDriving && IsAutonomous())
	{
		isDriving = mRobot->DriveDistance(7000,0.6);	//first parameter is encoder tick number, second speed
		Wait(0.02);
	}

	while((mShooter->IsLowerLimit() == false || mShooter->GetRaiseEncoder() < AUTONOMOUS::SHOOTER_ANGLE_DOWN) && IsAutonomous())
	{
		mShooter->LowerShooter();
		Wait(0.02);
	}
	mShooter->StopShooter();

	isDriving = true;
	mRobot->ResetDriveEncoder();
	mTimer->Reset();
	mTimer->Start();
	while(isDriving && IsAutonomous())
	{
		isDriving = mRobot->DriveDistance(5000,0.5);	//first parameter is encoder tick number, second speed
		if(mTimer->Get() >= 0.25)
			mShooter->RaiseShooter();
		Wait(0.02);
	}
	mShooter->StopShooter();
	mTimer->Stop();
}

void Robot::AutoPortDCullis()
{
	bool isDriving = true;
	mRobot->ResetDriveEncoder();
	while(isDriving && IsAutonomous())
	{
		isDriving = mRobot->DriveDistance(6000,0.6);	//first parameter is encoder tick number, second speed
		Wait(0.02);
	}

	while((mShooter->IsLowerLimit() == false || mShooter->GetRaiseEncoder() < AUTONOMOUS::SHOOTER_ANGLE_DOWN) && IsAutonomous())
	{
		mShooter->LowerShooter();
		Wait(0.02);
	}
	mShooter->StopShooter();

	isDriving = true;
	mRobot->ResetDriveEncoder();
	mTimer->Reset();
	mTimer->Start();
	while(isDriving && IsAutonomous())
	{
		isDriving = mRobot->DriveDistance(5000,0.5);	//first parameter is encoder tick number, second speed
		if(mTimer->Get() >= 0.15)
			mShooter->RaiseShooter();
		Wait(0.02);
	}
	mShooter->StopShooter();
	mTimer->Stop();
}

void Robot::AutoLowBar()
{
	while((mShooter->IsLowerLimit() == false || mShooter->GetRaiseEncoder() < AUTONOMOUS::SHOOTER_ANGLE_DOWN) && IsAutonomous())
	{
		mShooter->LowerShooter();
		Wait(0.02);
	}
	AutoDrive(6.0);
}

void Robot::Autonomous() //aligns with vision target then shoots
{
	enum autoType {kNormal, kCheval, kPortcullis, kLowBar, kNothing};
	autoType autoMode = kNormal;
	float angleAdjust = 0;
	int targetingResult;

	//Get info from AutoSelect4237
	ASPosition = mAutoSelect->getPosition();
	ASObstacle = mAutoSelect->getObstacle();
	ASSpeed = mAutoSelect->getSpeed();

	//GetAutoMode (defense type)
	SmartDashboard::PutString("DB/String 0", "Defense Slot Number: ");
	SmartDashboard::PutString("DB/String 1", "Defense Type Name: ");
	std::string mode = SmartDashboard::GetString("DB/String 5","0");
	std::string type = SmartDashboard::GetString("DB/String 6", "N");

	/*
	if(type[0] == 'C' || type[0] == 'c')
		autoMode = kCheval;
	else if(type[0] == 'P' || type[0] == 'p')
		autoMode = kPortDCullis;
	else if(type[0] == 'L' || type[0] == 'l')
		autoMode = kLowBar;
	else if(type == "DEATH") //Change this to something useful
		autoMode = kNothing;
	else
		autoMode = kNormal;
	 */

	if(ASObstacle == "Cheval De Frise")
		autoMode = kCheval;
	else if (ASObstacle == "Portcullis")
		autoMode = kPortcullis;
	else if (ASObstacle == "Low Bar")
		autoMode = kLowBar;
	else if (type == "None")
		autoMode = kNothing;
	else
		autoMode = kNormal;

	mLight->Set(true);

	std::cout << "Auto mode: " << ASPosition << std::endl << "Defense Type: " << ASObstacle << std::endl;

	switch(ASPosition)
	{
	case 1: //low bar
		printf("Position 1 (Low Bar)\n");

		angleAdjust = -80.0;

		break;

	case 2: //second from edge
		printf("Position 2 (Between middle and low bar)\n");

		angleAdjust = -20.0;

		break;

	case 3: //middle
		printf("Position 3 (Middle)\n");

		angleAdjust = -15.0;

		break;

	case 4: //second from secret passage
		printf("Position 4 (Between middle and secret passage)\n");

		angleAdjust = 10.0;

		break;

	case 5: //closest to secret passage
		printf("Position 5 (Next to secret passage)\n");

		angleAdjust = 20.0;

		break;

	default:
		printf("Mode Default\n");
		//DO NOTHING, SOMEONE MIGHT DIE (due to these unhelpful comments)
		while(IsAutonomous() && IsEnabled()){Wait(0.01);};
		break;
	}

	if(autoMode != kNothing)
	{
		//Could check for all the way down instead of forcing it for time duration
		AutoLowerShooter(0.8);
		Wait(0.3);

		if(autoMode == kCheval)
		{
			printf("Cheval De Frise\n");
			AutoCheval();

		}
		else if(autoMode == kPortcullis)
		{
			printf("Portcullis\n");
			AutoPortDCullis();
		}
		else if(autoMode == kLowBar)
		{
			printf("Low Bar\n");
			AutoLowBar();
		}
		else
		{
			printf("Generic Obstacle\n");
			AutoDrive(3.0);
		}
		Wait(0.7);

		if(mGyro->IsWorking() && IsAutonomous())
			AutoAdjustToGyro(angleAdjust);
		if(IsAutonomous())
			Wait(0.3);

		AutoRaiseForVision();
		if(IsAutonomous())
			Wait(0.3);

		targetingResult = AutoTargeting();
		if(IsAutonomous())
			Wait(0.5);

		AutoRaiseForShooting();
		if(IsAutonomous())
			Wait(0.3);

		mLight->Set(false);
		if((IsAutonomous() && targetingResult == 1) || DriverStation::GetInstance().GetMatchTime() > 13.0)
		{
			mShooter->ShootBall(-0.92);//Shoot ball
		}
	}

	mLight->Set(false);
}

int Robot::Targeting()
{
	// does not loop
	// returns 0 if drive train starts or continues to run to align camera and target
	// returns 1 if drive train stopped on target
	// returns 999 if any problem in vision - no targeting possible. motors maintain
	//There is a deadband near the center of the target
	float distFromCenter = mVision->UpdateVision();

	if(distFromCenter == 999.0) //error, don't do vision
		return 999;

	//distFromCenter += 3;

	if(distFromCenter >= 4.0) //vision target too far right, spin right
	{
		mRobot->TankDrive(-AUTONOMOUS::ROTATE_SPEED, AUTONOMOUS::ROTATE_SPEED);
		//printf("You must turn left! Dist from center = %f\n",distFromCenter);
	}
	else if(distFromCenter <= -4.0) //vision target too far left, spin left
	{
		mRobot->TankDrive(AUTONOMOUS::ROTATE_SPEED, -AUTONOMOUS::ROTATE_SPEED);
		//printf("You must turn right! Dist from center = %f\n",distFromCenter);
	}
	else
	{
		mRobot->ArcadeDrive(0.0,0.0); //stop, centered within 15 pixels
		printf("Centered!\n");
		return 1;
	}
	return 0;
}

void Robot::Test() //tests aligning with vision target
{
	//int aligned = 0;
	mLight->Set(true);

	while(IsTest() && IsEnabled())
	{
		int testMode = 0;
		if (testMode == 1)
		{
			std::cout << "Selected Drive Test" << std::endl;
			mRobot->Test(mStick);
			std::cout << "Exiting Drive Test" << std::endl;
		}
		else if (testMode == 2)
		{
			std::cout << "Selected Arm Test" << std::endl;
			mArm->Test(mStick);
			std::cout << "Exiting Arm Test" << std::endl;
		}
		else if (testMode == 3)
		{
			std::cout << "Selected Shooter Test" << std::endl;
			mShooter->Test(mStick);
			std::cout << "Exiting Shooter Test" << std::endl;
		}
		else if (testMode == 4)
		{
			std::cout << "Selected Gyro Test" << std::endl;
			mGyro->Test(mStick);
			std::cout << "Exiting Gyro Test" << std::endl;
		}
		else
		{
			std::cout << "Error" << std::endl;
		}
		Wait(0.75);
	}
}

START_ROBOT_CLASS(Robot);
