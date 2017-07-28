// Author: Darryl Wong, Yash Got'em
// Date: 1/21/17

// Revision History: 1/28/17 Added code to GetEncoder and StopClimbing functions


#ifndef CLIMBER_H
#define CLIMBER_H

#include <memory>
#include <PowerDistributionPanel.h>

class CANTalon;
namespace frc
{
	class Timer;
}

class Climber
{
public:
	Climber(int masterClimber, int slaveClimber);
	void ClimbRope(double speed);
	int GetEncoder();
	void StopClimbing();
	void Print();
private:
	std::shared_ptr<CANTalon> mMasterClimber;
	std::shared_ptr<CANTalon> mSlaveClimber;
	std::shared_ptr<frc::PowerDistributionPanel> mPDP;
	std::shared_ptr<frc::Timer> mAmpTimer;
};

#endif
