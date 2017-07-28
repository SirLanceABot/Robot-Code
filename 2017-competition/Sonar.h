// Author: Darryl Wong, Andrew Measel, Yash Got'em
// Date: 1/18/17

// Revision History:
// 1/21 DW: added get distance function + VOLTS_PER_5_MM and INCHES_IN_MM constants

#ifndef SONAR_H
#define SONAR_H

#include <memory>

namespace frc
{
class AnalogInput;
}

class Sonar
{
public:
	Sonar(int port);
	double GetDistance();
	void Print();

private:
	std::shared_ptr<frc::AnalogInput>mSonar;
};

#endif
