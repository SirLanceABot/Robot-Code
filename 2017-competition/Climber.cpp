// Author: Darryl Wong, Yash Got'em, Erin Lafrenz
// Date: 1/21/17

// Revision History: 1/28/17 Added code to GetEncoder and StopClimbing functions

//FIXME: uncomment lines 15, 24, 25, 26 for one motor only

#include "Climber.h"
#include "WPILib.h"
#include "CANTalon.h"

// initialize slave climber talon to master
Climber::Climber(int masterClimber, int slaveClimber):
mMasterClimber(std::make_shared<CANTalon>(masterClimber)),
mSlaveClimber(std::make_shared<CANTalon>(slaveClimber)),
mPDP(std::make_shared<PowerDistributionPanel>()),
mAmpTimer(std::make_shared<Timer>())
{
	std::cout << "[Climber]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;

	mMasterClimber->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	mSlaveClimber->ConfigNeutralMode(frc::CANSpeedController::kNeutralMode_Brake);
	mSlaveClimber->SetControlMode(CANSpeedController::kFollower);
	mSlaveClimber->Set(masterClimber);
	mMasterClimber->SetEncPosition(0);

	mMasterClimber->SetStatusFrameRateMs(CANTalon::StatusFrameRateAnalogTempVbat, 10);
	mSlaveClimber->SetStatusFrameRateMs(CANTalon::StatusFrameRateAnalogTempVbat, 10);

	mMasterClimber->SetStatusFrameRateMs(CANTalon::StatusFrameRateGeneral, 10);
	mSlaveClimber->SetStatusFrameRateMs(CANTalon::StatusFrameRateGeneral, 10);

	mAmpTimer->Stop();
	mAmpTimer->Reset();
}

// return the value of the master encoder
int Climber::GetEncoder()
{
	return -mMasterClimber->GetEncPosition();
}

// Set the speed of the climber motor
void Climber::ClimbRope(double speed)
{
//	if(speed < 0)
//		speed *= -1;
	double masterCurrent = mMasterClimber->GetOutputCurrent(); //mPDP->GetCurrent(12);
	double slaveCurrent = mSlaveClimber->GetOutputCurrent(); //mPDP->GetCurrent(13);
	//double PDPVoltage = mPDP->GetVoltage();
	double masterVoltage = mMasterClimber->GetBusVoltage();
	double slaveVoltage = mSlaveClimber->GetBusVoltage();

	std::cout << "Master Amperage: " << masterCurrent << " Slave Amperage: " << slaveCurrent << std::endl;
	std::cout << " Master Voltage: " << masterVoltage << " Slave Voltage: " << slaveVoltage << std::endl;
	std::cout << "      Amp Timer: " << mAmpTimer->Get() << std::endl;

	if (masterCurrent < 50.0 && slaveCurrent < 50.0)
	{
		mAmpTimer->Stop();
		mAmpTimer->Reset();
	}
	else if ((masterCurrent >= 50.0 || slaveCurrent >= 50.0) && mAmpTimer->Get() > 0.2)
	{
		std::cout << "Amperage Timer has been exceeded" << std::endl;
		//speed *= 0.5;
	}
	else if (masterCurrent >= 50.0 || slaveCurrent >= 50.0)
	{
		mAmpTimer->Start();
	}


	mMasterClimber->Set(-speed);
}

// stop the climber from climbing
void Climber::StopClimbing()
{
	mMasterClimber->Set(0);
}

// print the values of the encoder position to the net console
void Climber::Print()
{
	//std::cout << "Climber Encoder: " << GetEncoder() << "" << std::endl;
	//std::cout << "Motor 1 Amperage: " << mPDP->GetCurrent(12) << " Motor 2 Amperage: " << mPDP->GetCurrent(13) << std::endl;
	//std::cout << "PDP Voltage: " << mPDP->GetVoltage() << std::endl;
}
