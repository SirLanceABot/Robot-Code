/*
 * SingleSolenoid.cpp
 *
 *  Created on: Jan 18, 2017
 *      Author: Proramming Julein T. and Erin L.
 */
#include "PneumaticsSingle.h"
#include "WPILib.h"

PneumaticsSingle::PneumaticsSingle(int port):
	mSolenoid(std::make_shared<frc::Solenoid>(port))
{
	std::cout << "[PneumaticsSingle]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

// Set solenoid to extend (in)
void PneumaticsSingle::Extend()
{
	mSolenoid->Set(true);
}

// Set solenoid to retract (out)
void PneumaticsSingle::Retract()
{
	mSolenoid->Set(false);
}

// Return if solenoid is extended or retracted
int PneumaticsSingle::GetPosition()
{
	if(mSolenoid->Get())
		return kExtended;
	else
		return kRetracted;
}
