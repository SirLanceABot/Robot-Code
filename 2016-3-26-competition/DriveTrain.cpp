/*
 * DriveTrain.cpp
 *
 *  Created on: Feb 8, 2016
 *      Author: 18jthrum
 */

#include "DriveTrain.h"
#include "WPIlib.h"
#include <math.h>
#include "Constants.h"
#include "XboxJoystick.h"

DriveTrain::DriveTrain(int frontLeft, int rearLeft, int frontRight, int rearRight):
mFrontLeftMotor(std::make_shared<CANTalon>(frontLeft)),
mRearLeftMotor(std::make_shared<CANTalon>(rearLeft)),
mFrontRightMotor(std::make_shared<CANTalon>(frontRight)),
mRearRightMotor(std::make_shared<CANTalon>(rearRight)),
mRobotDrive(std::make_shared<RobotDrive>(mFrontLeftMotor, mRearLeftMotor, mFrontRightMotor, mRearRightMotor)),
mDoneDriving(true)
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

//	mFrontLeftMotor = new CANTalon(frontLeft);
	mFrontLeftMotor->SetFeedbackDevice(CANTalon::QuadEncoder);
	mFrontLeftMotor->SetEncPosition(0);

//	mRearLeftMotor = new CANTalon(rearLeft);

//	mFrontRightMotor = new CANTalon(frontRight);
	mFrontRightMotor->SetFeedbackDevice(CANTalon::QuadEncoder);
	mFrontRightMotor->SetSensorDirection(true); // invert to match left
	mFrontRightMotor->SetEncPosition(0);

//	mRearRightMotor = new CANTalon(rearRight);

//	mRobotDrive = new RobotDrive(mFrontLeftMotor, mRearLeftMotor, mFrontRightMotor, mRearRightMotor);
	mRobotDrive->SetSafetyEnabled(false);
}

DriveTrain::~DriveTrain()
{
//	delete mRobotDrive;
//	delete mRearRightMotor;
//	delete mFrontRightMotor;
//	delete mRearLeftMotor;
//	delete mFrontLeftMotor;
}

void DriveTrain::TankDrive(float leftspeed, float rightspeed)
{
	mRobotDrive->TankDrive(leftspeed, rightspeed);
}

void DriveTrain::ArcadeDrive(float move, float rotate)
{
	// -move = y axis up is positive & y axis down is negative
	mRobotDrive->ArcadeDrive(move, -rotate * DRIVE_TRAIN::SLOW_ROTATE);
//	if(fabs(move) > 0.2 || fabs(rotate) > 0.2)
//	{
//		//printf("Drive Encoder Left: %d, Right: %d\n", mFrontLeftMotor->GetEncPosition(), mFrontRightMotor->GetEncPosition());
//	}
}

void DriveTrain::ModifiedTankDrive(float left, float right)
{
	float min;

	if (right > 0 && left > 0 && (fabs(right - left) < .3))
	{
		min = fmin(right, left);
		TankDrive(min, min);
	}
	else if (right < 0 && left < 0 && (fabs(right - left) < .3))
	{
		min = fmax(right, left);
		TankDrive(min, min);
	}
	else if(right > 0 && left > 0 && (fabs(right - left) >= .3))
	{
		if (right > left)
		{
			TankDrive(left + .1 , right - .1);
		}
		else if (left > right)
		{
			TankDrive(left - .1 , right + .1);
		}
	}
	else if(right < 0 && left < 0 && (fabs(right - left) >= .3))
	{
		if (right > left)
		{
			TankDrive(left + .1 , right - .1);
		}
		else if (left > right)
		{
			TankDrive(left - .1 , right + .1);
		}
	}

	else if(right == 0 && left != 0)
	{
		TankDrive(left, right);
	}

	else if(right != 0 && left == 0)
	{
		TankDrive(left, right);
	}

	else if(right < 0 && left > 0)
	{
		min = fmin(-right, left);
		TankDrive(min, -min);
	}
	else if(right > 0 && left < 0)
	{
		min = fmin(right, -left);
		TankDrive(-min, min);
	}
	else
	{
		min = 0;
		TankDrive(min, min);
	}
	//printf("\nLeft Y Axis:  %f  Right Y Axis: %f", left, right);
}


void DriveTrain::ResetDriveEncoder()
{
	mFrontLeftMotor->SetEncPosition(0);
	mFrontRightMotor->SetEncPosition(0);
}

int DriveTrain::GetRightDriveEncoder()
{
	return mFrontRightMotor->GetEncPosition();
}

int DriveTrain::GetLeftDriveEncoder()
{
	return mFrontLeftMotor->GetEncPosition();
}

void DriveTrain::Stop()
{
	mRobotDrive->ArcadeDrive(0.0, 0.0);
}


bool DriveTrain::DriveDistance(int distance, float speed)
{
	// speed is sent negative or positive value, distance is positive

	int ticks;

	ticks = abs(mFrontLeftMotor->GetEncPosition());

	if(ticks >= distance) //4750
	{
		mDoneDriving = false;
		Stop();
	}
	else
	{
		TankDrive(speed, speed);
		mDoneDriving = true;
	}
	return mDoneDriving;
}

bool DriveTrain::Spin180()
{
	//Find the distance on the encoder if the robot was to turn 180 degrees (half the circumference)
	if(mFrontLeftMotor->CANTalon::GetEncPosition() >= (DRIVE_TRAIN::REVOLUTION_FOR_180_TURN * 1000))
	{
		mDoneDriving = true;
		Stop();
	}
	else
	{
		ArcadeDrive(0.0, 0.5);
		mDoneDriving = false;
	}
	return (mDoneDriving);
}

void DriveTrain::Test(std::shared_ptr<XboxJoystick> xbox)
{
	float leftY;
	float leftX;
	bool printMenu = true;

	while(xbox->GetX() == false)
	{
		if(printMenu == true)
		{
			printf("Press X to Exit\n");
			printf("Press A and Left Y to drive left side\n");
			printf("Press B and Left Y to drive right side\n");
			printf("Press Y and Left Y to drive both sides\n");
			printMenu = false;
		}

		while(xbox->GetA())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetA())
			{
				mFrontLeftMotor->Set(leftY);
				mRearLeftMotor->Set(leftY);
				printf("left motor speed = %f  , ",leftY);
				printf("Left encoder = %d\n",mFrontLeftMotor->GetEncPosition());
			}
			else
			{
				mFrontLeftMotor->Set(0);
				mRearLeftMotor->Set(0);
			}
		}
		mFrontLeftMotor->Set(0);
		mRearLeftMotor->Set(0);

		while(xbox->GetB())
		{
			leftY = xbox->GetLeftYAxis();
			printMenu = true;
			if(leftY != 0 && xbox->GetB())
			{
				mFrontRightMotor->Set(leftY);
				mRearRightMotor->Set(leftY);
				printf("Right motor speed = %f  , ",leftY);
				printf("Right encoder = %d\n",mFrontRightMotor->GetEncPosition());
			}
			else
			{
				mFrontRightMotor->Set(0);
				mRearRightMotor->Set(0);
			}
		}
		mFrontRightMotor->Set(0);
		mRearRightMotor->Set(0);

		while(xbox->GetY())
		{
			leftY = xbox->GetLeftYAxis();
			leftX = xbox->GetLeftXAxis();
			printMenu = true;
			if((leftY != 0 || leftX != 0) && xbox->GetY())
			{
				//mFrontRightMotor->Set(leftY);
				//mRearRightMotor->Set(leftY);
				//mFrontLeftMotor->Set(leftY);
				//mRearLeftMotor->Set(leftY);
				ArcadeDrive(leftY, leftX);
				printf("Drive speed = %f  , ",leftY);
				printf("Left encoder = %d  , ",mFrontLeftMotor->GetEncPosition());
				printf("Right encoder = %d\n",mFrontRightMotor->GetEncPosition());
			}
			else
			{
				//mFrontRightMotor->Set(0);
				//mRearRightMotor->Set(0);
				//mFrontLeftMotor->Set(0);
				//mRearLeftMotor->Set(0);
				ArcadeDrive(0.0, 0.0);
			}
		}
		//mFrontRightMotor->Set(0);
		//mRearRightMotor->Set(0);
		//mFrontLeftMotor->Set(0);
		//mRearLeftMotor->Set(0);
		ArcadeDrive(0.0, 0.0);
		Wait(0.02);
	}
}


