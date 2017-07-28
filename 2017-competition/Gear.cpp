/*
 * Gear.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: Programmig
 *      Revision History: MB, AS made if-else structure into switch structure, made timer part of the frc class
 */
#include "Gear.h"
#include "WPILib.h"
#include "PneumaticsDouble.h"

Gear::Gear(int gearHolderClose, int gearHolderOpen, int kickerOut, int kickerIn):
mGear(std::make_shared<PneumaticsDouble>(gearHolderClose, gearHolderOpen)),
mKicker(std::make_shared<PneumaticsDouble>(kickerOut, kickerIn)),
mTimer(std::make_shared<frc::Timer>()),
mCallDropGear(false),
mDropStage(kOpenHolder)
{
	std::cout << "[Gear]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}
//Closes the gear holder
void Gear::CloseGearHolder()
{
	mGear->Extend();
}

//Opens the gear holder
void Gear::OpenGearHolder()
{
	mGear->Retract();
}

//Kicks the gear of the robot
void Gear::KickerOut()
{
	mKicker->Extend();
}

void Gear::KickerIn()
{
	mKicker->Retract();
}

//Drops gear onto spring
bool Gear::DropGear()
{
	if(mCallDropGear == false)
	{
		mCallDropGear = true;
		mTimer->Reset();
		mTimer->Start();
	}

	switch(mDropStage)
	{
	case kOpenHolder:
		OpenGearHolder();
		if(mTimer->Get() >= GEAR_HOLDER_TIME)
			mDropStage = kKickOut;
		break;
	case kKickOut:
		KickerOut();
		if(mTimer->Get() >= GEAR_HOLDER_TIME + KICK_TIME)
			mDropStage = kKickIn;
		break;
	case kKickIn:
		KickerIn();
		if(mTimer->Get() >= GEAR_HOLDER_TIME + KICK_TIME + KICK_TIME)
		{
			mDropStage = kOpenHolder;
			mCallDropGear = false;
		}
		break;
	case kCloseHolder:
		//CloseGearHolder();
		//if(mTimer->Get() >= GEAR_HOLDER_TIME + KICK_TIME + KICK_TIME + GEAR_HOLDER_TIME)
		//{
		//	mDropStage = kOpenHolder;
		//	mCallDropGear = false;
		//	}
		break;
	}
	return mCallDropGear;
}
