/* Author: Mark Washington
 * Date: 2017
 * Revision History: MB made variables into enums
 */

#ifndef LIGHTRING_H
#define LIGHTRING_H

#include <memory>
/*
 * Value - Voltage
 *     0 -  6.8v
 *     1 -  8.4v
 *     2 - 10.0v
 *     3 - 11.6v
 *     4 - 13.2v
 *     5 - 13.5v
 *     6 - 13.5v
 *     7 - 13.5v
 */

namespace frc
{
	class DigitalOutput;
}

class LightRing
{
public:
	enum Voltage{k068v, k084v, k100v, k116v, k132v, k135v};
	LightRing(int OnOffPort, int foursPort, int twosPort, int onesPort);
	void SetIntensity(Voltage level);
    void TurnLightRingOn();
    void TurnLightRingOff();
private:
    std::shared_ptr<frc::DigitalOutput> mOnOffPin;
    std::shared_ptr<frc::DigitalOutput> mOnes;
    std::shared_ptr<frc::DigitalOutput> mTwos;
    std::shared_ptr<frc::DigitalOutput> mFours;
};



#endif /* SRC_LIGHTRING_H_ */
