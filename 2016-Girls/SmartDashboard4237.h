/*
 * SmartDashboard.h
 *
 *  Created on: Mar 21, 2016
 *      Author: Proramming
 */

#ifndef SMARTDASHBOARD_H_
#define SMARTDASHBOARD_H_

#include "WPIlib.h"
#include "PDP4237.h"

class SmartDashboard4237
{
public:
	SmartDashboard4237();
	~SmartDashboard4237();
	void PrintAmps();
	void NetworkTablePrint(std::string, float);
	void ChooseAuto(int &, bool &, bool &, bool &, bool &, bool &, bool &);
	void SetAuto();

private:
	std::shared_ptr<PDP4237> mPDP;
	std::shared_ptr<NetworkTable> mNetworkTable;
};

#endif /* SMARTDASHBOARD_H_ */
