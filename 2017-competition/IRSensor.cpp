// Author: Aaron Stoll
// Date: 1/24/17
// Revision History:
// AS 9/2/17: Added a identifier to the mIRSensor->GetOversampleBits outputs. Added File, Date, Time code. Added my data to how I got the conversion measurements. Changed Test() to Print().

#include "IRSensor.h"
#include "WPILib.h"


IRSensor::IRSensor(int port):
mIRSensor(std::make_shared<frc::AnalogInput>(port))
{
	std::cout << "[IRSensor]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;

	std::cout << "OversampledBits " << mIRSensor->GetOversampleBits() << "  ";
	std::cout << "AverageBits " << mIRSensor->GetAverageBits() << "  ";
	std::cout << "SampleRate " << mIRSensor->GetSampleRate() << std::endl;
}

// returns the distance in inches
// AvgVal 1955, 1435, 1120, 916, 640, 516, 400, 323
// Distance 4, 6, 8, 10, 15, 20, 25, 30
// power regression
double IRSensor::GetDistance()
{
	return 21521.2959 * pow(mIRSensor->GetAverageValue(), -1.1273);
}

// Displays the average value and average voltage, which are about the amount of inches away you are from surface.
void IRSensor::Print()
{
	std::cout << GetDistance() << std::endl;
}
