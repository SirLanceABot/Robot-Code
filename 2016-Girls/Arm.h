/*
 * Arm.h
 *
 *  Created on: Jan 25, 2016
 *      Author: Mark
 */

#ifndef ARM_H_
#define ARM_H_

#include <memory>
#include "AnalogTrigger.h"
#include "AnalogTriggerOutput.h"

// TODO:: UPDATE WHEN ARM IS FINISHED

class CANTalon;
class DigitalInput;
class XboxJoystick;
class Counter;
class AnalogTrigger;

class Arm
{
public:
	Arm(int, int, int, int, int);
	~Arm();

	enum AutoArm {kRaisingToVertical, kExtending, kRaisingPastVertical, kRetracting, kLowering, kDone};

	void RaiseArm();
	void LowerArm();
	int AttachHookAutonomously();
	void ResetHookProcess();
	void ExtendArm();
	void RetractArm();
	void PullWinch();
	void ReleaseWinch();
	bool ReadArmPivot();
	bool ReadArmExtend();
	void ArmRLStop(); 	// Stop Raising and lowering of arm
	void ArmERStop(); 	// Stop Extending and retracting of arm
	void StopWinch();   // Stop the winch from moving

	float CheckDirectionChange(float);
	int GetPosition();

	void Test(std::shared_ptr<XboxJoystick>);

private:
	std::shared_ptr<CANTalon> mRaiseMotor;
	std::shared_ptr<CANTalon> mExtendMotor;
	std::shared_ptr<CANTalon> mLeftWinchMotor;
	std::shared_ptr<CANTalon> mRightWinchMotor;
	Counter* mCounter; // count the encoder pulse triggers in current direction
	AnalogTrigger mAnalogTrigger; // create an encoder pulse trigger
	float mSpeedPrevious; // to remember previous direction
	int mPosition; // position accumulator to remember previous position before last direction change

//	CANTalon* mRaiseMotor;
//	CANTalon* mExtendMotor;
//	CANTalon* mLeftWinchMotor;
//	CANTalon* mRightWinchMotor;

	//DigitalInput* mVerticalLimit;

	float mRaiseSpeed;
	float mLowerSpeed;
	float mExtendSpeed;
	float mRetractSpeed;
	float mWinchPullSpeed;
	float mWinchReleaseSpeed;
	AutoArm mArmStep;
};

#endif /* SRC_ARM_H_ */
