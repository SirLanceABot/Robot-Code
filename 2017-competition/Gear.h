/*
 * Gear.h
 *
 *  Created on: Jan 25, 2017
 *      Author: Julien
 */

#ifndef GEAR_H
#define GEAR_H

#include "PneumaticsDouble.h"

namespace frc
{
	class Timer;
}

class Gear
{
public:
	enum DropStage {kOpenHolder, kKickOut, kKickIn, kCloseHolder};
	static constexpr double GEAR_HOLDER_TIME = 0.3;
	static constexpr double KICK_TIME = 0.4;
	Gear(int gearHolderClose, int gearHolderOpen, int kickerOut, int kickerIn);
	void OpenGearHolder();
	void CloseGearHolder();
	void KickerIn();
	void KickerOut();
	bool DropGear();
private:
	std::shared_ptr<PneumaticsDouble> mGear;
	std::shared_ptr<PneumaticsDouble> mKicker;
	std::shared_ptr<frc::Timer> mTimer;
	bool mCallDropGear;
	DropStage mDropStage;
};

#endif /* GEAR_H */
