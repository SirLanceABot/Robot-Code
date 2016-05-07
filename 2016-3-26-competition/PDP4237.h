/*
 * PDP4237.h
 *
 *  Created on: Feb 23, 2016
 *      Author: Programming
 */

#ifndef PDP4237_H_
#define PDP4237_H_

#include <memory>

class PowerDistributionPanel;

class PDP4237
{
public:
	PDP4237();
	~PDP4237();
	void MonitorPDP();
	double GetLeftDriveMaster();  // Returns the voltage of the Left Drive master in amps
	double GetLeftDriveFollow();  // Returns the voltage of the Left Drive follower in amps
	double GetLeftShootMaster();  //Returns the voltage of the Left Shoot Master in amps
	double GetLeftWinchMaster();  // Returns the voltage of the Left winch master in amps
	double GetShooterLift();      // Returns the voltage of the shooter Lift in amps
	double GetPusher();              // Returns the voltage of the Pusher in amps
	double GetLightRingPower();        // Returns the voltage of the Light ring power in amps
	double GetRightDriveMaster();         // Returns the voltage of the right drive master in amps
	double GetRightDriveFollow();          // Returns the voltage of the right drive follower in amps
	double GetRightShootMaster();         // Returns the voltage of the right shoot master in amps
	double GetRightWinchMaster();         // Returns the voltage of the right winch master in amps
	double GetArmPivot();                 // Returns the voltage of the arm pivot in amps
	double GetArmExtend();                // Returns the voltage of the arm extend in amps
	double GetPDPPort(int);
	bool IsLeftDriveMasterOk();
	bool IsLeftDriveFollowOk();
	bool IsLeftShootMasterOk();
	bool IsLeftWinchMasterOk();
	bool IsShooterLiftOk();
	bool IsPusherOk();
	bool IsLightRingPowerOk();
	bool IsRightDriveMasterOk();
	bool IsRightDriveFollowOk();
	bool IsRightShootMasterOk();
	bool IsRightWinchMasterOk();
	bool IsArmPivotOk();
	bool IsArmExtendOk();
private:
	std::shared_ptr<PowerDistributionPanel> mPDP;
//	PowerDistributionPanel* mPDP;
};

#endif /* SRC_PDP4237_H_ */
