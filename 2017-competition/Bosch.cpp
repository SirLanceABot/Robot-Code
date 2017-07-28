// Author: Darryl Wong
// Date: 2/25/17

// Position of BOSCH AHC-2 12V  6004.RA3.194-06 174.9:1 gear w/ encoder 1 tick per motor revolution on roboRIO analog 5 volt bus

#include "Bosch.h"
#include "WPILib.h"

Bosch::Bosch(int CANTalonDeviceID):

	mBosch(std::make_shared<CANTalon>(CANTalonDeviceID)),
	mTicks(0),
	mPreviousState(kHigh),
	mCurrentState(kHigh)
{
	std::cout << "[Bosch]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
	mBosch->SetFeedbackDevice(CANTalon::AnalogEncoder);
	mBosch->ConfigPotentiometerTurns(10);
	mBosch->SetEncPosition(0);
	mBosch->SetSafetyEnabled(false);
}

// Gets the ADC value of the motor
int Bosch::GetPosition()
{
	return mBosch->GetAnalogInRaw();
}

// Sets the motor speed and counts up encoder values
void Bosch::SetSpeed(double speed)
{
	if(speed < 0)
		speed *= -1;
	mBosch->Set(speed);

	int pos = GetPosition();

		if(pos > 800)
			mCurrentState = kHigh;
		else if(pos < 700)
			mCurrentState = kLow;
		else
			mCurrentState = kMiddle;

	if(mPreviousState == kMiddle && mCurrentState != kMiddle)
	{
		mTicks++;
	}
	mPreviousState = mCurrentState;
}

// Outputs the Encoder values
void Bosch::Print()
{
	std::cout << "Encoder: " << mTicks << std::endl;
}


