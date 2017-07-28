/* Author: Aaron Stoll
 * Date: 1/24/17
 * Revision History:
 * AS 9/2/17: Changed Test() to Print()
 */

#ifndef IRSENSOR_H
#define IRSENSOR_H

#include <memory>

namespace frc
{
class AnalogInput;
}

//class IRSensor that include the IRSensor function, the GetDistance function, and the Test function.
class IRSensor
{
public:
	IRSensor(int port);

	double GetDistance();

	void Print();
private:
	std::shared_ptr<frc::AnalogInput> mIRSensor;

};



#endif
