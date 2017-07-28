/*
 * Author: Erin Lafrenz, Julien Thrum
 *
 * Date: 1/28/2017
 * Revision History: 1/28/2016 Changed to PneumaticsSingle convention of extendPort and Retract Port from forward and reverse
 * 							   Created enum for the return statements in GetPosition()
 */


#ifndef PNEUMATICSDOUBLE_H
#define PNEUMATICSDOUBLE_H

#include <memory>

namespace frc
{
class DoubleSolenoid;
}

class PneumaticsDouble
{
public:
	enum Position{kRetracted = -1, kOff, kExtended};
	PneumaticsDouble(int extendPort, int retractPort);
	void Extend();
	void Retract();
	void Off();
	int GetPosition();
private:
	std::shared_ptr<frc::DoubleSolenoid>mDoubleSolenoid;
};



#endif /* DOUBLESOLENOID_H_ */
