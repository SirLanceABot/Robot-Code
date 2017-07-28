// Authors: Aaron Stoll, Andrew Measel
// Date: 28/1/17
// Revision History:
// 31/1/17 Aaron Stoll: Made shooter controllable by Pot 1 on the Switchboard

#include "Shooter.h"
#include "WPILib.h"
#include "CANTalon.h"

Shooter::Shooter(int shooter, int shooterGate):
mShooter(std::make_shared<CANTalon>(shooter)),
mGate(std::make_shared<DigitalOutput>(shooterGate)),
mShooterCodesPerRev(80),
mRPMSpeed(0),
mVoltSpeed(0)
{
	std::cout << "[Shooter]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;

	mShooter->SetFeedbackDevice(CANTalon::FeedbackDevice::QuadEncoder);
	mShooter->ConfigEncoderCodesPerRev(mShooterCodesPerRev);
//	mShooter->SetControlMode();
	mGate->Set(false);
}

// Set the speed of the motor
void Shooter::ShootBall(double speed)
{
	mShooter->Set(speed);
	if(speed == 0)
	{
		mGate->Set(false);
	}
	else
	{
		mGate->Set(true);
	}
}

// Sets the RPM of the motor
void Shooter::SetSpeedRPM(double RPMSpeed)
{
	mRPMSpeed = RPMSpeed * 6000;
}

void Shooter::ShootBallRPM()
{
	mShooter->Set(mRPMSpeed);
}

void Shooter::StopBallRPM()
{
	mShooter->Set(0);
}

// Sets the voltage of the motor
void Shooter::SetSpeedVolt(double VoltSpeed)
{
	mVoltSpeed = VoltSpeed * 14;
}

void Shooter::ShootBallVolt()
{
	mShooter->Set(mVoltSpeed);
}

void Shooter::StopBallVolt()
{
	mShooter->Set(0);
}
