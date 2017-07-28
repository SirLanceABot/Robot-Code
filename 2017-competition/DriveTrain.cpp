/*
 * DriveTrain.cpp
 *
 *  Created on: Jan 19, 2017
 *      Author: Programming
 */
#include "DriveTrain.h"
#include "WPILib.h"
#include "CANTalon.h"
//#include "Constants.h"

DriveTrain::DriveTrain(int leftMaster, int leftSlave1, int leftSlave2, int rightMaster, int rightSlave1, int rightSlave2, int lowSpeed, int highSpeed, frc::I2C::Port gyroPort, double sensitivity):
mGyro(std::make_shared<ITG3200>(gyroPort, sensitivity)),
mLeftMaster(std::make_shared<CANTalon>(leftMaster)),
mLeftSlave1(std::make_shared<CANTalon>(leftSlave1)),
//mLeftSlave2(std::make_shared<CANTalon>(leftSlave2)),
mRightMaster(std::make_shared<CANTalon>(rightMaster)),
mRightSlave1(std::make_shared<CANTalon>(rightSlave1)),
//mRightSlave2(std::make_shared<CANTalon>(rightSlave2)),
mGearShift(std::make_shared<PneumaticsDouble>(lowSpeed, highSpeed)),
mDrive(std::make_shared<frc::RobotDrive>(mLeftMaster, mRightMaster)),
mIsDoneDriving(true),
mPosition(kLow)

{
	std::cout << "[DriveTrain]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << '\n';

	mRightSlave1->SetControlMode(CANSpeedController::kFollower);
	mRightSlave1->Set(rightMaster);
	//	mRightSlave2->SetControlMode(CANSpeedController::kFollower);
	//	mRightSlave2->Set(rightMaster);

	mLeftSlave1->SetControlMode(CANSpeedController::kFollower);
	mLeftSlave1->Set(leftMaster);
	//	mLeftSlave2->SetControlMode(CANSpeedController::kFollower);
	//	mLeftSlave2->Set(leftMaster);





	mRightMaster->SetFeedbackDevice(CANTalon::QuadEncoder);
	mRightMaster->SetInverted(true);
	mRightMaster->ConfigEncoderCodesPerRev(ENCODER_CODES_PER_REV);
	mRightMaster->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateFeedback, 8);
	mRightMaster->SetVoltageRampRate(240.0);

	mLeftMaster->SetFeedbackDevice(CANTalon::QuadEncoder);
	mLeftMaster->SetInverted(true);
	mLeftMaster->ConfigEncoderCodesPerRev(ENCODER_CODES_PER_REV);
	mLeftMaster->SetStatusFrameRateMs(CANTalon::StatusFrameRate::StatusFrameRateFeedback, 8);
	mLeftMaster->SetVoltageRampRate(240.0);
	ResetEncoders();

	mRightMaster->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	mRightSlave1->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	//	mRightSlave2->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	mLeftMaster->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	mLeftSlave1->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	//	mLeftSlave2->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
}

bool DriveTrain::GetGearPos()
{
	//return true for high gear
	if(mPosition == kLow)
		return false;
	else
		return true;
}


void DriveTrain::SetRampRate(int rate)
{
	mLeftMaster->SetVoltageRampRate(rate);
	mRightMaster->SetVoltageRampRate(rate);
}

//Makes the mIsDoneDriving variable true
void DriveTrain::MakeIsDoneDrivingTrue()
{
	mIsDoneDriving = true;
}

// Call Arcade Drive
void DriveTrain::ArcadeDrive(double move, double rotate, bool square)
{
	mDrive->ArcadeDrive(-move, rotate, square);
}

// Call Tank Drive
void DriveTrain::TankDrive(double left, double right)
{
	mDrive->TankDrive(-left, -right);
}

// Shifts Gear
void DriveTrain::ShiftSpeed()
{
	if(mPosition == kLow)
	{
		ShiftUp();
		//mPosition = kHigh;
	}
	else
	{
		ShiftDown();
		//mPosition = kLow;
	}

}

// Shifts Down
// With no air pressure, automatically shifts into low gear
void DriveTrain::ShiftDown()
{
	mGearShift->Extend();
	mPosition = kLow;
}

// Shifts Up
void DriveTrain::ShiftUp()
{
	mGearShift->Retract();
	mPosition = kHigh;
}

// Get Left Encoder Position
int DriveTrain::GetLeftEncoder()
{
	return mLeftMaster->GetEncPosition() * -1;
}

// Get Right Encoder position
int DriveTrain::GetRightEncoder()
{
	return mRightMaster->GetEncPosition();
}

// Print encoder postions
void DriveTrain::Print()
{
	if (mPosition == kLow)
	{
		std::cout << "mPosition: low ";
	}
	else if (mPosition == kHigh)
	{
		std::cout << "mPosition: high";
	}

	std::cout << " GetPosition is " << mGearShift->GetPosition() << '\n';
}

// Drives certain distance
bool DriveTrain::DriveDistance(double speed, double distance)
{
	if (mIsDoneDriving)
	{
		mIsDoneDriving = false;
		ResetEncoders();
		Wait(0.1);
		TankDrive(speed, speed);
	}
	else if (GetDistance() >= distance)
	{
		StopDriving();
		ResetEncoders();
		Wait(0.1);
		mIsDoneDriving = true;
	}



	return mIsDoneDriving;
}

std::string DriveTrain::GetShifterPos()
{
	if(mPosition == kLow)
	{
		return "Low";
	}
	else
	{
		return "High";
	}
}

bool DriveTrain::NewDriveDistance(double speed, double distance)
{
	if (mIsDoneDriving)
	{
		mIsDoneDriving = false;
		ResetEncoders();
		TankDrive(speed, speed);
		std::cout << distance << '\n';
	}
	else if (fabs(mLeftMaster->GetPosition()) >= distance || fabs(mRightMaster->GetPosition()) >= distance)
	{
		StopDriving();
		ResetEncoders();
		mIsDoneDriving = true;
	}
	else
	{
		TankDrive(speed, speed);
	}

	return mIsDoneDriving;
}

// Tells if safety is enabled
void DriveTrain::SetSafetyEnabled(bool safety)
{
	mDrive->SetSafetyEnabled(safety);
}

// Get Distance driven from encoders
double DriveTrain::GetDistance()
{
	// 25.2627908 is ticks per inch driven
	// (360 * 4 / 3) / (6 * PI) = 25.2627908
	int enc = GetLeftEncoder();
	if (enc < 0)
		return (double)-enc / TICKS_PER_INCH;
	else
		return (double)enc / TICKS_PER_INCH;
}

// Spin counterclock-wise
bool DriveTrain::SpinCCW(double speed, double angle)
{
	if (mIsDoneDriving)
	{
		mIsDoneDriving = false;
		ResetEncoders();
		Wait(0.1);
		TankDrive(-speed, speed);
	}
	else if (GetDistance() >= (angle / 360.0) * WHEEL_BASE_CIRCUMFERENCE)
	{
		StopDriving();
		ResetEncoders();
		Wait(0.1);
		mIsDoneDriving = true;
	}

	return mIsDoneDriving;
}

bool DriveTrain::NewSpinCCW(double speed, double angle)
{
	if(mGyro->IsWorking())
	{
		if(mIsDoneDriving)
		{
			mIsDoneDriving = false;
			ResetGyro();
			TankDrive(-speed, speed);
		}
		else if(mGyro->GetZ() >= angle)
		{
			StopDriving();
			mIsDoneDriving = true;
		}
		else
		{
			TankDrive(-speed, speed);
		}
	}
	else
	{
		if (mIsDoneDriving)
		{
			mIsDoneDriving = false;
			ResetEncoders();
			TankDrive(-speed, speed);
		}
		else if (fabs(mLeftMaster->GetPosition()) >= (angle / 360.0) * WHEEL_BASE_CIRCUMFERENCE || fabs(mRightMaster->GetPosition()) >= (angle / 360.0) * WHEEL_BASE_CIRCUMFERENCE)
		{
			StopDriving();
			ResetEncoders();
			mIsDoneDriving = true;
		}
		else
		{
			TankDrive(-speed, speed);
		}
	}

	return mIsDoneDriving;
}

// Spin clockwise
bool DriveTrain::SpinCW(double speed, double angle)
{
	if (mIsDoneDriving)
	{
		mIsDoneDriving = false;
		ResetEncoders();
		Wait(0.1);
		TankDrive(speed, -speed);
	}
	else if (GetDistance() >= (angle / 360.0) * WHEEL_BASE_CIRCUMFERENCE)
	{
		StopDriving();
		ResetEncoders();
		Wait(0.1);
		mIsDoneDriving = true;
	}

	return mIsDoneDriving;
}

bool DriveTrain::NewSpinCW(double speed, double angle)
{
	if(mGyro->IsWorking())
	{
		if(mIsDoneDriving)
		{
			mIsDoneDriving = false;
			ResetGyro();
			TankDrive(speed, -speed);
		}
		else if(fabs(mGyro->GetZ()) >= angle)
		{
			StopDriving();
			mIsDoneDriving = true;
		}
		else
		{
			TankDrive(speed, -speed);
		}
	}
	else
	{
		if (mIsDoneDriving)
		{
			mIsDoneDriving = false;
			ResetEncoders();
			TankDrive(speed, -speed);
		}
		else if (fabs(mLeftMaster->GetPosition()) >= (angle / 360.0) * WHEEL_BASE_CIRCUMFERENCE || fabs(mRightMaster->GetPosition()) >= (angle / 360.0) * WHEEL_BASE_CIRCUMFERENCE)
		{
			StopDriving();
			ResetEncoders();
			mIsDoneDriving = true;
		}
		else
		{
			TankDrive(speed, -speed);
		}
	}

	return mIsDoneDriving;
}

// Sets control mode to percent v bus
void DriveTrain::SetControlModePercentVBus()
{
	mLeftMaster->SetControlMode(mLeftMaster->ControlMode::kPercentVbus);
	mRightMaster->SetControlMode(mRightMaster->ControlMode::kPercentVbus);
	mDrive->SetMaxOutput(1.0);
	Wait(0.005);
}

// Sets control mode to voltage
void DriveTrain::SetControlModeVoltage()
{
	mLeftMaster->SetControlMode(mLeftMaster->ControlMode::kVoltage);
	mRightMaster->SetControlMode(mRightMaster->ControlMode::kVoltage);
	mDrive->SetMaxOutput(12.5);
	Wait(0.005);
}

// Returns true or false for if gyro is working
bool DriveTrain::IsGyroWorking()
{
	return mGyro->IsWorking();
}

// Reset encoders
void DriveTrain::ResetEncoders()
{
	mRightMaster->SetPosition(0);
	mLeftMaster->SetPosition(0);
}

void DriveTrain::SetGyroAngle(double angleX, double angleY, double angleZ)
{
	double angle[] = {angleX, angleY, angleZ};
	mGyro->SetAngle(angle);
}

// Stops Driving
void DriveTrain::StopDriving()
{
	TankDrive(0, 0);
}

//Get gyro yaw
double DriveTrain::GetZ()
{
	return mGyro->GetZ();
}

// Resets gyro to 0, 0, 0
void DriveTrain::ResetGyro()
{
	SetGyroAngle(0.0, 0.0, 0.0);
}
