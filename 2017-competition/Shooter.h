// Authors: Aaron Stoll, Andrew Weasel
// Date: 28/1/17
// Revision History:
// 31/1/17 Aaron Stoll: Made shooter controllable by Pot 1 on the Switchboard

#ifndef SHOOTER_H
#define SHOOTER_H

#include <memory>

namespace frc
{
	class DigitalOutput;
}

class CANTalon;

class Shooter
{
public:
	Shooter(int shooter, int shooterGate);

	void ShootBall(double speed);
	void SetSpeedRPM(double RPMSpeed);
	void ShootBallRPM();
	void StopBallRPM();
	void SetSpeedVolt(double Volt);
	void ShootBallVolt();
	void StopBallVolt();
private:
	std::shared_ptr<CANTalon> mShooter;
	std::shared_ptr<frc::DigitalOutput> mGate;
	short int mShooterCodesPerRev; //CIMcoder
	double mRPMSpeed;
	double mVoltSpeed;
};


#endif
