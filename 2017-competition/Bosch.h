// Author: Darryl Wong
// Date: 2/25/17

#ifndef BOSCH_H
#define BOSCH_H

// Position of BOSCH AHC-2 12V  6004.RA3.194-06 174.9:1 gear w/ encoder 1 tick per motor revolution on roboRIO analog 5 volt bus

#include "CANTalon.h"
#include <memory>

class Bosch
{
public:
	enum State{kHigh, kMiddle, kLow};
	Bosch(int CANTalonDeviceID);
	int GetPosition();
	void SetSpeed(double speed);
	void Print();


private:
	std::shared_ptr<CANTalon>mBosch; // motor
	int mTicks;

	State mPreviousState;
	State mCurrentState;
};

#endif
