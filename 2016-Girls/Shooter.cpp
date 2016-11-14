/*

 * Shooter.cpp
 *
 *  Created on: Feb 7, 2015
 *      Author: Michael Conard
 **/

#include "Shooter.h"
#include "Constants.h"
#include "WPILib.h"
#include "XboxJoystick.h"

Shooter::Shooter(int leftShooter, int rightShooter, int raisePort, int pushServoPort):
mLeftShooterTalon(std::make_shared<CANTalon>(leftShooter)),
mRightShooterTalon(std::make_shared<CANTalon>(rightShooter)),
mRaiseTalon(std::make_shared<CANTalon>(raisePort)),
mPush(std::make_shared<CANTalon>(pushServoPort))
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

//	mLeftShooterTalon = new CANTalon(leftShooter);
	mLeftShooterTalon->SetFeedbackDevice(CANTalon::QuadEncoder);
	mLeftShooterTalon->SetEncPosition(0);
	mLeftShooterTalon->SetControlMode(CANSpeedController::kVoltage);

	//	mRightShooterTalon = new CANTalon(rightShooter);
	mRightShooterTalon->SetFeedbackDevice(CANTalon::QuadEncoder);
	mRightShooterTalon->SetSensorDirection(true); //invert to match left
	mRightShooterTalon->SetEncPosition(0);
	mRightShooterTalon->SetControlMode(CANSpeedController::kVoltage);

//	mRaiseTalon = new CANTalon(raisePort);
	mRaiseTalon->ConfigNeutralMode(CANSpeedController::kNeutralMode_Brake);
	mRaiseTalon->ConfigLimitMode(CANSpeedController::kLimitMode_SwitchInputsOnly);
	mRaiseTalon->ConfigFwdLimitSwitchNormallyOpen(true);
	mRaiseTalon->ConfigRevLimitSwitchNormallyOpen(true);
	mRaiseTalon->SetFeedbackDevice(CANTalon::QuadEncoder);
	mRaiseTalon->SetEncPosition(0);

//	mPush = new CANTalon(pushServoPort);
	mPush->ConfigLimitMode(CANSpeedController::kLimitMode_SoftPositionLimits);
	mPush->ConfigForwardLimit(AUTONOMOUS::SHOOTER_PUSHER_FORWARD); //total range of 350
	mPush->ConfigReverseLimit(AUTONOMOUS::SHOOTER_PUSHER_REVERSE); //starting point in middle
	mPush->SetFeedbackDevice(CANTalon::QuadEncoder);
	mPush->SetEncPosition(0);
	mPush->SetControlMode(CANSpeedController::kVoltage);
	mPush->SetVoltageRampRate(150);
}

Shooter::~Shooter()
{
//	delete mPush;
//	delete mLeftShooterTalon;
//	delete mRightShooterTalon;
//	delete mRaiseTalon;
}

void Shooter::GrabBall() //suck in ball
{
	ActivateMotors(0.55 * 12.5);
}

void Shooter::ShootBall(float speed)
{
	speed *= 12.5;
	ActivateMotors(speed); //speed up fly wheels (negative to shoot)
	Wait(0.8);
	Shoot();
	Wait(0.35);
	mPush->Set(0.0);
	DisableMotors(); //turn off fly wheels
	//ResetPushEnc();
}

void Shooter::Shoot()
{
	mPush->Set(0.75 * 12.5);
	Wait(0.35);
	mPush->Set(-0.75 * 12.5);
}

void Shooter::ActivateMotors(float direction)
{
	if (direction < 0) //negative for shooting
	{
		//printf("Grabbing ball, Left Wheel Enc: %d, Right Wheel Enc: %d\n", mLeftShooterTalon->GetEncPosition(), mRightShooterTalon->GetEncPosition());
		mLeftShooterTalon->Set(direction);
		mRightShooterTalon->Set(direction);
	}
	if (direction > 0) //positive for suck in
	{
		//printf("Shooting ball, Left Wheel Enc: %d, Right Wheel Enc: %d\n", mLeftShooterTalon->GetEncPosition(), mRightShooterTalon->GetEncPosition());
		mLeftShooterTalon->Set(direction);
		mRightShooterTalon->Set(direction);
	}
	printf("Encoder Position: %d",mRaiseTalon->GetEncPosition());
}

void Shooter::DisableMotors() //turn off fly wheels
{
	mLeftShooterTalon->Set(0.0);
	mRightShooterTalon->Set(0.0);
}

void Shooter::RaiseShooter() //raise shooter angle
{
	mRaiseTalon->Set(0.40); // Was 0.7
	//printf("Encoder: %d\n", mRaiseTalon->GetEncPosition());
}

void Shooter::LowerShooter() //lower shooter angle
{
	mRaiseTalon->Set(-0.40); // Was -0.35
	//printf("Encoder: %d\n", mRaiseTalon->GetEncPosition());
}

void Shooter::ResetRaiseEnc()
{
	mRaiseTalon->SetEncPosition(0);
}

int Shooter::GetRaiseEncoder()
{
	return mRaiseTalon->GetEncPosition();
}

bool Shooter::RaiseToShootAngle()
{
	bool done = true;
	int shooterAngle = abs(mRaiseTalon->GetEncPosition());
	printf("Shooter Enc: %d\n", shooterAngle);

	if(fabs(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_SHOOT) <= AUTONOMOUS::SHOOTER_ANGLE_TOLERANCE)
	{
		mRaiseTalon->Set(0.0);
		done = false;
	}
	else if(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_SHOOT < 0)
		mRaiseTalon->Set(-.37);
	else if(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_SHOOT > 0)
		mRaiseTalon->Set(.37);

	return done;
}

bool Shooter::RaiseToVisionAngle()
{
	bool done = true;
	int shooterAngle = abs(mRaiseTalon->GetEncPosition());
	printf("Shooter Enc: %d\n", shooterAngle);

	if(fabs(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_VISION) <= AUTONOMOUS::SHOOTER_ANGLE_TOLERANCE)
	{
		mRaiseTalon->Set(0.0);
		done = false;
	}
	else if(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_VISION < 0)
		mRaiseTalon->Set(-.44);
	else if(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_VISION > 0)
		mRaiseTalon->Set(.44);

	return done;
}

bool Shooter::RaiseToJamShotAngle()
{
	bool done = true;
	int shooterAngle = abs(mRaiseTalon->GetEncPosition());
	printf("Shooter Enc: %d\n", shooterAngle);

	if(fabs(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_JAM_SHOT) <= AUTONOMOUS::SHOOTER_ANGLE_TOLERANCE)
	{
		mRaiseTalon->Set(0.0);
		done = false;
	}
	else if(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_JAM_SHOT < 0)
		mRaiseTalon->Set(-.37);
	else if(shooterAngle - AUTONOMOUS::SHOOTER_ANGLE_JAM_SHOT > 0)
		mRaiseTalon->Set(.37);

	return done;
}

void Shooter::StopShooter() //disable shooter
{
	mRaiseTalon->Set(0.0);
}

void Shooter::HoldShooter()
{
	mRaiseTalon->Set(0.15);
}

bool Shooter::IsUpperLimit()
{
	if (mRaiseTalon->IsFwdLimitSwitchClosed() == 1)
	{
		//printf("Upper limit has been hit\n");
		ResetRaiseEnc();
		return true;
	}
	else
	{
		return false;
	}
}

bool Shooter::IsLowerLimit()
{
	if (mRaiseTalon->IsRevLimitSwitchClosed() == 1)
	{
		//printf("Lower limit has been hit\n");
		//mRaiseTalon->SetEncPosition(0);
		return true;
	}
	else
	{
		return false;
	}
}

void Shooter::ResetPushEnc()
{
	mPush->SetEncPosition(0);
}

void Shooter::TestPusher(float speed)
{
	mPush->Set(speed);
	//printf("Pusher Encoder: %d\n", mPush->GetEncPosition());
}

void Shooter::Test(std::shared_ptr<XboxJoystick> xbox)
{
	float leftY;
	bool printMenu = true;

	while(xbox->GetX() == false)
	{
		if(printMenu == true)
		{
			printf("Press X to Exit\n");
			printf("Press A and Left Y to shoot or grab ball\n");
			printf("Press B and Left Y to raise or lower shooter\n");
			printf("Press Y and Left Y to activate pusher\n");
			printMenu = false;
		}

		while(xbox->GetA())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetA())
			{
				mLeftShooterTalon->Set(leftY * 12.5);
				mRightShooterTalon->Set(leftY * 12.5);
				printf("Shooter speed = %f  , ",leftY  * 12.5);
				printf("Left shooter encoder = %d  , ",mLeftShooterTalon->GetEncPosition());
				printf("Right shooter encoder = %d\n",mRightShooterTalon->GetEncPosition());
			}
			else
			{
				mLeftShooterTalon->Set(0);
				mRightShooterTalon->Set(0);
			}
		}
		mLeftShooterTalon->Set(0);
		mRightShooterTalon->Set(0);

		while(xbox->GetB())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetB())
			{
				mRaiseTalon->Set(leftY * 12.5);
				printf("Raise shooter speed = %f  , ",leftY);
				printf("Raise shooter encoder = %d\n",mRaiseTalon->GetEncPosition());
			}
			else
			{
				mRaiseTalon->Set(0);
			}
		}
		mRaiseTalon->Set(0);

		while(xbox->GetY())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetY())
			{
				mPush->Set(leftY * 12.5);
				printf("Pusher speed = %f  , ",leftY);
				printf("Pusher encoder = %d\n",mPush->GetEncPosition());
			}
			else
			{
				mPush->Set(0);
			}
		}
		mPush->Set(0);

		Wait(0.02);
	}
}

/*
void Shooter::TestResetPusher()
{
	printf("Setting pusher to -0.5\n");
	mPush->Set(-0.5);
	Wait(1);
	mPush->SetEncPosition(0);
}*/
