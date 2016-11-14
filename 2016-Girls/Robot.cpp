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
	std::shared_ptr<Vision> mVision;
	std::shared_ptr<Arm> mArm;
	std::shared_ptr<DigitalOutput> mLight;
	std::shared_ptr<DriveTrain> mRobot;
	std::shared_ptr<ITG3200> mGyro;  //gyro
	std::shared_ptr<Timer> mTimer;
	NetworkTable* mNetworkTable;
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

	//AutoSelect4237 variables start with "AS4237"
	int AS4237Position = 6;
	std::string AS4237Obstacle = "None";
	int AS4237Speed = 1.0;
	double AS4237Orientation = 0.0;
	bool AS4237EnableArm = true;

};

Robot::Robot():
												mStick(std::make_shared<XboxJoystick>(XBOX::DRIVER_PORT)),
												mShooter(std::make_shared<Shooter>(CAN_PORT::SHOOTER_LEFT_WHEEL, CAN_PORT::SHOOTER_RIGHT_WHEEL, CAN_PORT::RAISE_SHOOTER, CAN_PORT::SHOOTER_PUSH)),
												mVision(std::make_shared<Vision>()),
												mArm(std::make_shared<Arm>(CAN_PORT::ARM_PIVOT, CAN_PORT::ARM_EXTEND, CAN_PORT::LEFT_WINCH, CAN_PORT::RIGHT_WINCH, 5)),
												mLight(std::make_shared<DigitalOutput>(DIO_PORT::LIGHT_RING)),
												mRobot(std::make_shared<DriveTrain>(CAN_PORT::FRONT_LEFT, CAN_PORT::REAR_LEFT, CAN_PORT::FRONT_RIGHT, CAN_PORT::REAR_RIGHT)),
												mTimer(std::make_shared<Timer>())//,

{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

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
}

void Robot::RobotInit() {
	//CameraServer::GetInstance()->SetQuality(50);
	//CameraServer::GetInstance()->StartAutomaticCapture("cam0");
	printf("Robot Init\n");

}


//void Robot::RobotInit()
//{
//	printf("Enter Init\n");
////	mNetworkTable = NetworkTable::GetTable("SmartDashboard");
//
//	// the following lines set the values for autonomous choosing
//	int autoMode = -1;
//	bool driveToDefense = true;
//	bool crossTheDefense = true;
//	bool target = true;
//	bool shoot = true;
//	bool done = false;
//	bool done2 = false;
//
////	SmartDashboard::PutNumber("Auto Mode", -1);
////
////	SmartDashboard::PutBoolean("Drive To Defense", 1);
////	SmartDashboard::PutBoolean("Cross the Defense", 1);
////	SmartDashboard::PutBoolean("Target", 1);
////	SmartDashboard::PutBoolean("Shoot", 1);
////	SmartDashboard::PutBoolean("Done", 1);
////	SmartDashboard::PutBoolean("Done2", 1);
//
//	mDashboard->SetAuto();
//
//	while(done == false || done2 == false)
//	{
//		mDashboard->ChooseAuto(autoMode, driveToDefense, crossTheDefense, target, shoot, done, done2);
////		printf("autoMode %d, driveToDefense %d, crossTheDefense %d, target %d, shoot %d, done %d, done2 %d\n",
////			autoMode, driveToDefense, crossTheDefense, target, shoot, done, done2);
////		fflush(NULL);
//		Wait(0.05);
//	}
//	printf("\nDone making decisions\n");
//
//	//	CANTalon::FeedbackDeviceStatus encoderStatus = mMotor->IsSensorPresent(CANTalon::CtreMagEncoder_Absolute);
//	//	switch(encoderStatus)
//	//	{
//	//	case CANTalon::FeedbackStatusPresent:
//	//
//	//		autoNameEncoder = "Encoder Found";
//	//		break;
//	//
//	//	case CANTalon::FeedbackStatusNotPresent:
//	//
//	//		autoNameEncoder = "Encoder Not Found";
//	//		break;
//	//
//	//	case CANTalon::FeedbackStatusUnknown:
//	//
//	//		autoNameEncoder = "Encoder Unknown";
//	//		break;
//	//	}
//	//
//	//		SmartDashboard::PutString(autoNameEncoder, autoNameEncoder);
//	printf("Exit Init\n");
//}

void Robot::OperatorControl() //standard driving and shooter control
{
	bool raisingToShoot = true;
	int doneTargeting = 0;
	//bool isDriving = true;
	int dPad = -1;
	int armProcess = 0;

	mLight->Set(false);

	while(IsOperatorControl() && IsEnabled())
	{
/**
		//The part where it looks at Networktables and changes stuff
		AS4237Position = mNetworkTable->GetNumber("Position");
		AS4237Obstacle = mNetworkTable->GetString("Obstacle");
		AS4237Speed = mNetworkTable->GetNumber("Speed 2016");
		AS4237Orientation = mNetworkTable->GetNumber("Orientation 2016");
		AS4237EnableArm = mNetworkTable->GetNumber("Enable Arm");
		printf("%d\n", AS4237Position);
		std::cout << AS4237Obstacle << "\n";
		printf("%f\n", AS4237Speed);
		printf("%f\n", AS4237Orientation);
		printf("%d\n", AS4237EnableArm);
		printf("\n\n\n\n");
*/




		//**************************************************************************************
		mRobot->ArcadeDrive(mStick->GetLeftYAxis()*1.0, mStick->GetLeftXAxis()); //standard drive

		//**************************************************************************************
		if((mStick->GetY()) /*|| mStick->GetRightYAxis() < -0.2)*/ && mShooter->IsUpperLimit() == false)
			mShooter->RaiseShooter();
		else if((mStick->GetA()) /*|| mStick->GetRightYAxis() > 0.2)*/ && mShooter->IsLowerLimit() == false)
			mShooter->LowerShooter();
		else
			mShooter->StopShooter();

		//mShooter->TestRaiseShooter();

		if(mShooter->IsUpperLimit() == true && mStick->GetA() == false)
		{
			mShooter->StopShooter();
		}
		if(mShooter->IsLowerLimit() == true && mStick->GetY() == false)
		{
			mShooter->StopShooter();
		}

		//mShooter->TestPusher(mStick->GetRightYAxis());

		//**************************************************************************************
		//		if(mStick->GetBack()) //back button
		//		{
		//			mLight->Set(true);
		//			Wait(0.3);
		//			doneTargeting = 0;
		//			while(doneTargeting == 0 && mStick->GetLeftStickPress() == false) //left stick press to cancel
		//				doneTargeting = Targeting(); //line up with the target
		//			mLight->Set(false);
		//		}

		//**************************************************************************************
		if(mStick->GetRightBumper())
		{
			raisingToShoot = true;
			while(raisingToShoot && mStick->GetLeftStickPress() == false) //left stick press to cancel
			{
				raisingToShoot = mShooter->RaiseToShootAngle(); //angle shooter to target
				printf("Aiming for Shooting\n");
			}
		}

		//		**************************************************************************************
		if(mStick->GetLeftBumper())
		{
			raisingToShoot = true;
			while(raisingToShoot && mStick->GetLeftStickPress() == false) //left stick press to cancel
			{
				raisingToShoot = mShooter->RaiseToVisionAngle(); //angle shooter to target
				printf("Aiming for Vision\n");
			}
		}

		//**************************************************************************************
		if(mStick->GetLeftTrigger() >= 0.5) //suck in
		{
			mShooter->GrabBall();
		}
		else if(mStick->GetRightTrigger() >= 0.5) //jam shot
		{
			mRobot->ResetDriveEncoder();
			Wait(0.1);

			raisingToShoot = true;
			//			isDriving = true;
			//			while((isDriving) && mStick->GetLeftStickPress() == false)
			//			{
			//				isDriving = mRobot->DriveDistance(4750, -.80);	//first parameter is encoder tick number, second speed
			//				//printf("performing jam shot\n");
			//				Wait(0.02);
			//			}
			//			mRobot->Stop();
			while((raisingToShoot) && mStick->GetLeftStickPress() == false)
			{
				raisingToShoot = mShooter->RaiseToJamShotAngle(); //angle shooter to target
				//printf("performing jam shot\n");
				Wait(0.02);
			}
			mShooter->StopShooter();
			//AutoAdjustToGyro(0.0);
			//Wait(0.3);

			//mShooter->ShootBall(-.60);
		}
		else if(mStick->GetX()) //low goal
		{
			mShooter->GrabBall();
			Wait(0.2);
			mRobot->Stop();
			//mLight ON
			mShooter->ShootBall(-0.63);
			//mLight OFF
		}
		else if(mStick->GetB()) //high goal
		{
			mShooter->GrabBall();
			Wait(0.2);
			mRobot->Stop();
			mShooter->ShootBall(-0.9);
		}
		else
		{
			mShooter->DisableMotors();
		}

		//**************************************************************************************
		//		dPad = mStick->GetDPad();
		//		if(dPad != -1 && mStick->GetRightStickPress())
		//		{
		//			if(dPad == 90)
		//				mArm->RaiseArm();
		//			else if(dPad == 270)
		//				mArm->LowerArm();
		//
		//			if(dPad == 0)
		//				mArm->ExtendArm();
		//			else if(dPad == 180)
		//				mArm->RetractArm();
		//		}
		//		else
		//		{
		//			mArm->ArmRLStop();
		//			mArm->ArmERStop();
		//		}
		//
		//		//**************************************************************************************
		//		if(mStick->GetStart())
		//		{
		//			while(armProcess != 2 && mStick->GetLeftStickPress() == false && IsEnabled()) //5 is kDone, 2 is kRaisingPastVeritcal
		//			{
		//				armProcess = mArm->AttachHookAutonomously();
		//			}
		//			mArm->ResetHookProcess();
		//			mArm->StopWinch();
		//			mArm->ArmRLStop();
		//			mArm->ArmERStop();
		//		}
		//
		//		//**************************************************************************************
		//		if(mStick->GetRightYAxis() >= 0.75)
		//		{
		//			while(mStick->GetRightYAxis() >= 0.75 && IsEnabled())
		//				mArm->PullWinch();
		//			mArm->StopWinch();
		//		}
		//		else if(mStick->GetRightYAxis() <= -0.75)
		//		{
		//			while(mStick->GetRightYAxis() <= -0.75 && IsEnabled())
		//				mArm->ReleaseWinch();
		//			mArm->StopWinch();
		//		}

		//Display the shooter encoder
		printf("Shooter Encoder: %d\n", mShooter->GetRaiseEncoder());

		Wait(0.02);
	}
}

void Robot::AutoLowerShooter(float time)
{
	mTimer->Start();
	while(mTimer->Get() < time && IsAutonomous())
		mShooter->LowerShooter();
	mShooter->StopShooter();

	mTimer->Stop();
	mTimer->Reset();
}

void Robot::AutoRaiseShooter(float time)
{
	mTimer->Start();
	while(mTimer->Get() < time && IsAutonomous())
		mShooter->RaiseShooter();
	mShooter->StopShooter();

	mTimer->Stop();
	mTimer->Reset();
}

void Robot::AutoAdjustToGyro(float angle)
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
	mRobot->ArcadeDrive(0.0,0.0);
}

void Robot::AutoDrive(float time)
{
	mTimer->Reset();
	mTimer->Start();
	while(mTimer->Get() < time && IsAutonomous())  //drive through outerworks
		mRobot->TankDrive(0.88, 0.88); //left is 1% weaker
	mRobot->ArcadeDrive(0.0,0.0);
	mTimer->Stop();
	mTimer->Reset();
}

void Robot::AutoRaiseForVision()
{
	bool raisingForVision = true;
	while(raisingForVision && IsAutonomous())
	{
		raisingForVision = mShooter->RaiseToVisionAngle();
		//printf("Aiming Shooter for Vision\n");
	}
}

void Robot::AutoRaiseForShooting()
{
	bool raisingToShoot = true;
	while(raisingToShoot && IsAutonomous())
	{
		raisingToShoot = mShooter->RaiseToShootAngle();
		//printf("Aiming Shooter for Shooting\n");
	}
}

int Robot::AutoTargeting()
{
	int doneTargeting = 0;
	while(doneTargeting == 0 && IsAutonomous())  //line up with the target
	{
		doneTargeting = Targeting();
		Wait(0.03);
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
	}

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
	}

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
	}
	AutoDrive(6.0);
}

void Robot::Autonomous() //aligns with vision target then shoots
{
	enum autoType {kNormal, kCheval, kPortDCullis, kLowBar, kNothing};
	autoType autoMode = kNormal;
	float angleAdjust = 0;
	int targetingResult;

	//GetAutoMode (defense type)
	SmartDashboard::PutString("DB/String 0", "Defense Slot Number: ");
	SmartDashboard::PutString("DB/String 1", "Defense Type Name: ");
	std::string mode = SmartDashboard::GetString("DB/String 5","0");
	std::string type = SmartDashboard::GetString("DB/String 6", "N");

	if(type[0] == 'C' || type[0] == 'c')
		autoMode = kCheval;
	else if(type[0] == 'P' || type[0] == 'p')
		autoMode = kPortDCullis;
	else if(type[0] == 'L' || type[0] == 'l')
		autoMode = kLowBar;
	else if(type [0] == 'D') //Change this to something useful
		autoMode = kNothing;
	else
		autoMode = kNormal;

	mLight->Set(true);

	std::cout << "Auto mode: " << mode << std::endl << "Defense Type: " << type << std::endl;

	switch(mode[0])
	{
	case '1': //low bar
		printf("Mode 1\n");

		angleAdjust = -80.0;

		break;
	case '2': //second from edge
		printf("Mode 2\n");

		angleAdjust = -20.0;

		break;
	case '3': //middle
		printf("Mode 3\n");

		angleAdjust = -15.0;

		break;
	case '4': //second from secret passage
		printf("Mode 4\n");

		angleAdjust = 10.0;

		break;
	case '5': //closest to secret passage
		printf("Mode 5\n");

		angleAdjust = 20.0;

		break;
	default:
		printf("Mode Default\n");
		//DO NOTHING, SOMEONE MIGHT DIE
		while(IsAutonomous() && IsEnabled()){Wait(0.01);};
		break;
	}

	if(autoMode != kNothing)
	{
		AutoLowerShooter(0.8); Wait(0.3);

		if(autoMode == kCheval)
			AutoCheval();
		else if(autoMode == kPortDCullis)
			AutoPortDCullis();
		else if(autoMode == kLowBar)
			AutoLowBar();
		else
			AutoDrive(3.0);
		/* Wait(0.7);

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

		//	AutoTargeting();
		//	if(IsAutonomous())
		//		Wait(0.3);

		AutoRaiseForShooting();
		if(IsAutonomous())
			Wait(0.3);

		mLight->Set(false);
		if(IsAutonomous() && targetingResult == 1)
			mShooter->ShootBall(-0.92);*/
	}

	mLight->Set(false);
}

int Robot::Targeting()
{
	float distFromCenter = mVision->UpdateVision();

	if(distFromCenter == 999) //error, don't do vision
		return 999;

	//distFromCenter += 3;

	if(distFromCenter >= 4) //vision target too far right, spin right
	{
		mRobot->TankDrive(-AUTONOMOUS::ROTATE_SPEED, AUTONOMOUS::ROTATE_SPEED);
		//printf("You must turn left! Dist from center = %f\n",distFromCenter);
	}
	else if(distFromCenter <= -4) //vision target too far left, spin left
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
	mLight->Set(1);

	RefreshMenu();

	while(IsTest() && IsEnabled())
	{
		int testMode = SelectTestMode();
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
		Wait(0.01);
	}
}

int Robot::SelectTestMode()
{
	bool IsChoosingMode = true;
	while (IsChoosingMode == true && IsEnabled())
	{
		if (mStick->GetPOV() == XBOX::POV_UP && CurrentArrow != 1)
		{
			while(mStick->GetPOV() == XBOX::POV_UP)
			{}
			CurrentArrow -= 1;
			ResetArrows();
			RefreshMenu();
		}
		if (mStick->GetPOV() == XBOX::POV_DOWN && CurrentArrow != MenuMax)
		{
			while(mStick->GetPOV() == XBOX::POV_DOWN)
			{}
			CurrentArrow += 1;
			ResetArrows();
			RefreshMenu();
		}
		if (mStick->GetRawButton(XBOX::B_BUTTON) == 1)
		{
			std::cout << "Selected " << CurrentArrow << std::endl;;
			Wait(1.0);

			IsChoosingMode = false;
		}

		if (CurrentArrow == 1)
			ArrowSlot1 = '>';
		else if (CurrentArrow == 2)
			ArrowSlot2 = '>';
		else if (CurrentArrow == 3)
			ArrowSlot3 = '>';
		else if (CurrentArrow == 4)
			ArrowSlot4 = '>';
		else if (CurrentArrow == 5)
			ArrowSlot5 = '>';
		else if (CurrentArrow == 6)
			ArrowSlot6 = '>';
		else if (CurrentArrow == 7)
			ArrowSlot7 = '>';
	}
	return CurrentArrow;
}

void Robot::RefreshMenu()
{
	for (int num = 0; num <= 100; num++)
	{
		std::cout << std::endl;
	}

	std::cout << "Current Arrow: " << CurrentArrow << std::endl;
	if (CurrentArrow == 1) std::cout << ">"; else std::cout << " ";
	std::cout <<"Drive Test" << std::endl;
	if (CurrentArrow == 2) std::cout << ">"; else std::cout << " ";
	std::cout << "Arm Test" << std::endl;
	if (CurrentArrow == 3) std::cout << ">"; else std::cout << " ";
	std::cout << "Shooter Test" << std::endl;
	if (CurrentArrow == 4) std::cout << ">"; else std::cout << " ";
	std::cout << "Gyro Test" << std::endl;
	if (CurrentArrow == 5) std::cout << ">"; else std::cout << " ";
	std::cout << "Vision Test" << std::endl;
	if (CurrentArrow == 6) std::cout << ">"; else std::cout << " ";
	std::cout << "Xbox Joystick Test" << std::endl;
	if (CurrentArrow == 7) std::cout << ">"; else std::cout << " ";
	std::cout << "Logitch Joystick Test" << std::endl;

}

void Robot::ResetArrows()
{
	ArrowSlot1 = ' ';
	ArrowSlot2 = ' ';
	ArrowSlot3 = ' ';
	ArrowSlot4 = ' ';
	ArrowSlot5 = ' ' ;
	ArrowSlot6 = ' ';
	ArrowSlot7 = ' ';
}

START_ROBOT_CLASS(Robot);
