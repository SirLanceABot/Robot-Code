// Author: Darryl Wong, Andrew Measel, Yash Got'em
// Date: 1/18/17

// Revision History:
// 1/21 DW: added get distance function
// 2/2 DW, YG, BD: Changed distance equation

#include "Sonar.h"
#include "WPILib.h"

Sonar::Sonar(int port):
mSonar(std::make_shared<frc::AnalogInput>(port))
{
	std::cout << "[Sonar]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

// Returns Distance in inches
// Values used for equation
// Volts: .29, .36, .42, .54, .63, .76, .84, .965, 1.15
// Inches: 12,  16,  20,  24,  28,  32,  36,   40,   44
double Sonar::GetDistance()
{
	return 41.384462991982 * pow(mSonar->GetVoltage(), .92756692760095);
}


void Sonar::Print()
{
	std::cout << "Inches: " << GetDistance() << std::endl;
	std::cout << "Voltage: " << mSonar->GetVoltage() << std::endl;
}
