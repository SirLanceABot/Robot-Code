/* Author: Mark Washington
 * Date: 2017
 * Revision History: MB made DigitalOutput part of frc, changed variable names
 */
#include "LightRing.h"
#include "WPILib.h"
#include <memory>

LightRing::LightRing(int OnOffPort, int foursPort, int twosPort, int onesPort):
	mOnOffPin (std::make_shared<frc::DigitalOutput>(OnOffPort)),
	mOnes (std::make_shared<frc::DigitalOutput>(onesPort)),
	mTwos (std::make_shared<frc::DigitalOutput>(twosPort)),
	mFours (std::make_shared<frc::DigitalOutput>(foursPort))
{
	std::cout << "[Light Ring]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
}

void LightRing::SetIntensity(Voltage level)
{
	int pins [3];
	pins[0] = (level/4) % 2; //Calculate value for pin 14
	pins[1] = (level/2) % 2; //Calculate value for pin 13
	pins[2] = level % 2;     //Calculate value for pin 12

	mFours->Set(pins[0]);     //Set intensity
	mTwos->Set(pins[1]);     //Set intensity
	mOnes->Set(pins[2]);     //Set intensity
}

void LightRing::TurnLightRingOn() //Turns light ring on
{
    mOnOffPin->Set(true); //Sets value of pin 10 to true, turning the light ring on
}

void LightRing::TurnLightRingOff() //Turns light ring off
{
    mOnOffPin->Set(false); //Sets value of pin 10 to false, turning the light ring off
}
