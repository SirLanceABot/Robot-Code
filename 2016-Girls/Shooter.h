/*
 * Shooter.h
 *
 *  Created on: Feb 7, 2015
 *      Author: Michael Conard
 **/

#ifndef SHOOTER_H_
#define SHOOTER_H_

#include <memory>


class CANTalon;
class AnalogInput;
class XboxJoystick;

class Shooter
{
public:
	Shooter(int, int, int, int); //Constructor
	~Shooter(); 							//Destructor

	void GrabBall();
	void ShootBall(float);
	void ActivateMotors(float);
	void DisableMotors();

	void RaiseShooter();
	void LowerShooter();
	bool RaiseToShootAngle();
	bool RaiseToVisionAngle();
	bool RaiseToJamShotAngle();
	int GetRaiseEncoder();

	void StopShooter();
	void HoldShooter();
	bool IsUpperLimit();
	bool IsLowerLimit();
	void Shoot();
	void ResetRaiseEnc();
	void ResetPushEnc();

	//void TestResetPusher();

	void TestPusher(float);
	void Test(std::shared_ptr<XboxJoystick>);
private:
	std::shared_ptr<CANTalon> mLeftShooterTalon;
	std::shared_ptr<CANTalon> mRightShooterTalon;
	std::shared_ptr<CANTalon> mRaiseTalon;
	std::shared_ptr<CANTalon> mPush;

//	CANTalon* mRightShooterTalon;
//	CANTalon* mLeftShooterTalon;
//	CANTalon* mRaiseTalon;
//	CANTalon* mPush;


};

#endif
