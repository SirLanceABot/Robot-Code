/*
 * SingleSolenoid.h
 *
 *  Created on: Jan 18, 2017
 *      Author: Proramming Julien T. and Erin L.
 */

#ifndef PNEUMATICSSINGLE_H
#define PNEUMATICSSINGLE_H

#include <memory>

namespace frc
{
class Solenoid;
}

class PneumaticsSingle
{
public:
	enum Position{kRetracted = -1, kExtended = 1};
	PneumaticsSingle(int port);
	void Extend();
	void Retract();
	int GetPosition();
private:
	std::shared_ptr<frc::Solenoid>mSolenoid;
};



#endif /* SINGLESOLENOID_H_ */
