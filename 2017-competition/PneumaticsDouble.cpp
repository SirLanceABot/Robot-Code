/*
 * Author: Erin Lafrenz, Julien Thrum
 *
 * Date: 1/28/2017
 * Revision History: 1/28/2016 Changed to PneumaticsSingle convention of extendPort and Retract Port from forward and reverse
 * 							   Created enum for the return statements in GetPosition()
 */
#include "PneumaticsDouble.h"
#include "WPILib.h"

PneumaticsDouble::PneumaticsDouble(int extendPort, int retractPort):
	mDoubleSolenoid(std::make_shared<frc::DoubleSolenoid>(extendPort, retractPort))
{
	std::cout << "[PneumaticsDouble]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

// set double solenoid to forward
void PneumaticsDouble::Extend()
{
	mDoubleSolenoid->Set(frc::DoubleSolenoid::kForward);
}

// set double solenoid to reverse
void PneumaticsDouble::Retract()
{
	mDoubleSolenoid->Set(frc::DoubleSolenoid::kReverse);
}

// set double solenoid to off
void PneumaticsDouble::Off()
{
	mDoubleSolenoid->Set(frc::DoubleSolenoid::kOff);
}

// Return ints based on the position of the double solenoid
int PneumaticsDouble::GetPosition()
{
	if (mDoubleSolenoid->Get() == frc::DoubleSolenoid::kReverse)
		return kRetracted;
	if (mDoubleSolenoid->Get() == frc::DoubleSolenoid::kForward)
		return kExtended;
	else
		return kOff;

}
