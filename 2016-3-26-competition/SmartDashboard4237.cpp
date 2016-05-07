/*
 * SmartDashboard.cpp
 *
 *  Created on: Mar 21, 2016
 *      Author: Proramming
 */
#include <SmartDashboard4237.h>
#include "WPIlib.h"
#include "PDP4237.h"

SmartDashboard4237::SmartDashboard4237():
	mPDP(std::make_shared<PDP4237>()),
	mNetworkTable(NetworkTable::GetTable("SmartDashboard"))
{
	//mNetworkTable = NetworkTable::GetTable("SmartDashboard");
}

SmartDashboard4237::~SmartDashboard4237()
{
	//delete mPDP;
}

void SmartDashboard4237::PrintAmps()
{
	NetworkTablePrint("Left Drive Master", mPDP->GetLeftDriveMaster());
	NetworkTablePrint("Left Drive Follow", mPDP->GetLeftDriveFollow());
	NetworkTablePrint("Left Shoot Master", mPDP->GetLeftShootMaster());
	NetworkTablePrint("Left Shoot Follow", mPDP->GetLeftWinchMaster());
	NetworkTablePrint("Shooter Lift", mPDP->GetShooterLift());
	NetworkTablePrint("Pusher", mPDP->GetPusher());
	NetworkTablePrint("Light Ring Power", mPDP->GetLightRingPower());
	NetworkTablePrint("Right Drive Master", mPDP->GetRightDriveMaster());
	NetworkTablePrint("Right Drive Follow", mPDP->GetRightDriveFollow());
	NetworkTablePrint("Right Shoot Master", mPDP->GetRightShootMaster());
	NetworkTablePrint("Right Winch Master", mPDP->GetRightWinchMaster());
	NetworkTablePrint("Arm Pivot", mPDP->GetArmPivot());
	NetworkTablePrint("Arm Extend", mPDP->GetArmExtend());

	//add boolean true/false green/red squares
	mNetworkTable->PutBoolean("Left Drive Master bool", mPDP->IsLeftDriveMasterOk());
	mNetworkTable->PutBoolean("Left Drive Follow bool", mPDP->IsLeftDriveFollowOk());
	mNetworkTable->PutBoolean("Left Shoot Master bool", mPDP->IsLeftShootMasterOk());
	mNetworkTable->PutBoolean("Left Shoot Follow bool", mPDP->IsLeftWinchMasterOk());
	mNetworkTable->PutBoolean("Shooter Lift bool", mPDP->IsShooterLiftOk());
	mNetworkTable->PutBoolean("Pusher bool", mPDP->IsPusherOk());
	mNetworkTable->PutBoolean("Light Ring Power bool", mPDP->IsLightRingPowerOk());
	mNetworkTable->PutBoolean("Right Drive Master bool", mPDP->IsRightDriveMasterOk());
	mNetworkTable->PutBoolean("Right Drive Follow bool", mPDP->IsRightDriveFollowOk());
	mNetworkTable->PutBoolean("Right Shoot Master bool", mPDP->IsRightShootMasterOk());
	mNetworkTable->PutBoolean("Right Winch Master bool", mPDP->IsRightWinchMasterOk());
	mNetworkTable->PutBoolean("Arm Pivot bool", mPDP->IsArmPivotOk());
	mNetworkTable->PutBoolean("Arm Extend bool", mPDP->IsArmExtendOk());
}

void SmartDashboard4237::NetworkTablePrint(std::string name, float value) //char name[21]
{
	char number[31];
	sprintf(number, "%5.1f", value);
	mNetworkTable->PutString(name, number);
}

void SmartDashboard4237::ChooseAuto(int& autoMode, bool& driveToDefense, bool& crossTheDefense, bool& target, bool& shoot, bool& done, bool& done2)
{
	printf("Enter Choose Auto\n");
	fflush(NULL);

	autoMode = mNetworkTable->GetNumber("Auto Mode", -1);
	driveToDefense = mNetworkTable->GetBoolean("Drive To Defense", true);
	crossTheDefense = mNetworkTable->GetBoolean("Cross the Defense", true);
	target = mNetworkTable->GetBoolean("Target", true);
	shoot = mNetworkTable->GetBoolean("Shoot", true);
	done = mNetworkTable->GetBoolean("Done", false);
	done2 = mNetworkTable->GetBoolean("Done2", false);

//	SmartDashboard::PutNumber("Auto Mode", autoMode);
//	SmartDashboard::PutBoolean("Drive To Defense", driveToDefense);
//	SmartDashboard::PutBoolean("Cross the Defense", crossTheDefense);
//	SmartDashboard::PutBoolean("Target", target);
//	SmartDashboard::PutBoolean("Shoot", shoot);
//	SmartDashboard::PutBoolean("Done", done);
//	SmartDashboard::PutBoolean("Done2", done2);

	printf("Exit Choose Auto\n");
	fflush(NULL);
}

void SmartDashboard4237::SetAuto()
{
	printf("Enter Set Auto\n");
	fflush(NULL);
//	SmartDashboard::PutNumber("Auto Mode", -1);
//	SmartDashboard::PutBoolean("Drive To Defense", true);
//	SmartDashboard::PutBoolean("Cross the Defense", true);
//	SmartDashboard::PutBoolean("Target", true);
//	SmartDashboard::PutBoolean("Shoot", true);
//	SmartDashboard::PutBoolean("Done", false);
//	SmartDashboard::PutBoolean("Done2", false);

	mNetworkTable->PutNumber("Auto Mode", -1);
	mNetworkTable->PutBoolean("Drive To Defense", true);
	mNetworkTable->PutBoolean("Cross the Defense", true);
	mNetworkTable->PutBoolean("Target", true);
	mNetworkTable->PutBoolean("Shoot", true);
	mNetworkTable->PutBoolean("Done", false);
	mNetworkTable->PutBoolean("Done2", false);
	printf("Exit Set Auto\n");
	fflush(NULL);
}
