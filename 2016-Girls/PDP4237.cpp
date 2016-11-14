/*
 * PDP4237.cpp
 *
 *  Created on: Feb 23, 2016
 *      Author: Programming
 */
#include "PDP4237.h"
#include "WPILib.h"
#include "Constants.h"

PDP4237::PDP4237():
	mPDP(std::make_shared<PowerDistributionPanel>())
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
//	mPDP = new PowerDistributionPanel();
}

PDP4237::~PDP4237()
{
}

void PDP4237::MonitorPDP()        //Gets the Current of the PDP Ports and Returns a message if it is above a certain amount
{
	double pdpLeftDriveMaster = mPDP->GetCurrent(PDP_PORTS::LEFT_DRIVE_MASTER);
	double pdpLeftDriveFollow = mPDP->GetCurrent(PDP_PORTS::LEFT_DRIVE_FOLLOW);
	double pdpLeftShootMaster = mPDP->GetCurrent(PDP_PORTS::LEFT_SHOOT_MASTER);
	double pdpLeftWinchMaster = mPDP->GetCurrent(PDP_PORTS::LEFT_WINCH_MASTER);
	double pdpShooterLift = mPDP->GetCurrent(PDP_PORTS::SHOOTER_LIFT);
	double pdpPusher = mPDP->GetCurrent(PDP_PORTS::PUSHER);
	double pdpLightRingPower = mPDP->GetCurrent(PDP_PORTS::LIGHT_RING_POWER);
	double pdpRightDriveMaster = mPDP->GetCurrent(PDP_PORTS::RIGHT_DRIVE_MASTER);
	double pdpRightDriveFollow = mPDP->GetCurrent(PDP_PORTS::RIGHT_DRIVE_FOLLOW);
	double pdpRightShootMaster = mPDP->GetCurrent(PDP_PORTS::RIGHT_SHOOT_MASTER);
	double pdpRightWinchMaster = mPDP->GetCurrent(PDP_PORTS::RIGHT_WINCH_MASTER);
	double pdpArmPivot = mPDP->GetCurrent(PDP_PORTS::ARM_PIVOT);
	double pdpArmExtend = mPDP->GetCurrent(PDP_PORTS::ARM_EXTEND);
	double TotalCurrent = mPDP->GetTotalCurrent();
//	printf("TotalCurrent  %f", TotalCurrent);

	//Not all warning values are correct, values only tested for CIM Motors

	if(pdpLeftDriveMaster > PDP_PORTS::CIM_MAX_AMPS)
	{
		printf("LeftDriverMaster is over 10 amps\n");
	}
	if(pdpLeftDriveFollow > PDP_PORTS::CIM_MAX_AMPS)
	{
		printf("LeftDriveFollow is over 10 amps\n");
	}
	if(pdpLeftShootMaster > PDP_PORTS::MINI_CIM_MAX_AMPS)
	{
		printf("LeftShootMaster is over 10 amps\n");
	}
	if(pdpLeftWinchMaster > PDP_PORTS::MINI_CIM_MAX_AMPS)
	{
		printf("LeftWinchMaster is over 10 amps\n");
	}
	if(pdpShooterLift > PDP_PORTS::MINI_CIM_MAX_AMPS)
	{
		printf("ShooterLift is over 10 amps\n");
	}
	if(pdpPusher > PDP_PORTS::PLANETARY_MAX_AMPS)
	{
		printf("Pusher is over 10 amps\n");
	}
	if(pdpLightRingPower > PDP_PORTS::LIGHT_RING_MAX_AMPS)
	{
		printf("LightRingpPower is over 10 amps\n");
	}
	if(pdpRightDriveMaster > PDP_PORTS::CIM_MAX_AMPS)
	{
		printf("RightDriveMaster is over 10 amps\n");
	}
	if(pdpRightDriveFollow > PDP_PORTS::CIM_MAX_AMPS)
	{
		printf("RightDriveFollow is over 1 amps\n");
	}
	if(pdpRightShootMaster > PDP_PORTS::MINI_CIM_MAX_AMPS)
	{
		printf("RightShootMaster is over 10 amps\n");
	}
	if(pdpRightWinchMaster > PDP_PORTS::MINI_CIM_MAX_AMPS)
	{
		printf("RightWinchMaster is over 10 amps\n");
	}
	if(pdpArmPivot > PDP_PORTS::WINDOW_MAX_AMPS)                   //UPDATE if switch to BOSCH
	{
		printf("ArmPivot is over 10 amps\n");
	}
	if(pdpArmExtend > PDP_PORTS::WINDOW_MAX_AMPS)                   //UPDATE if switch to BOSCH
	{
		printf("ArmExtend is over 10 amps\n");
	}
	if(TotalCurrent > PDP_PORTS::TOTAL_PDP_LIMIT)
	{
		printf("TotalCurrent is over 100 amps\n");
	}
}

double PDP4237::GetLeftDriveMaster()
{
	return mPDP->GetCurrent(PDP_PORTS::LEFT_DRIVE_MASTER);
}

double PDP4237::GetLeftDriveFollow()
{
	return mPDP->GetCurrent(PDP_PORTS::LEFT_DRIVE_FOLLOW);
}
double PDP4237::GetLeftShootMaster()
{
	return mPDP->GetCurrent(PDP_PORTS::LEFT_SHOOT_MASTER);
}
double PDP4237::GetLeftWinchMaster()
{
	return mPDP->GetCurrent(PDP_PORTS::LEFT_WINCH_MASTER);
}
double PDP4237::GetShooterLift()
{
	return mPDP->GetCurrent(PDP_PORTS::SHOOTER_LIFT);
}
double PDP4237::GetPusher()
{
	return mPDP->GetCurrent(PDP_PORTS::PUSHER);
}
double PDP4237::GetLightRingPower()
{
	return mPDP->GetCurrent(PDP_PORTS::LIGHT_RING_POWER);
}

double PDP4237::GetRightDriveMaster()
{
	return mPDP->GetCurrent(PDP_PORTS::RIGHT_DRIVE_MASTER);
}

double PDP4237::GetRightDriveFollow()
{
	return mPDP->GetCurrent(PDP_PORTS::RIGHT_DRIVE_FOLLOW);
}

double PDP4237::GetRightShootMaster()
{
	return mPDP->GetCurrent(PDP_PORTS::RIGHT_SHOOT_MASTER);
}

double PDP4237::GetRightWinchMaster()
{
	return mPDP->GetCurrent(PDP_PORTS::RIGHT_WINCH_MASTER);
}

double PDP4237::GetArmPivot()
{
	return mPDP->GetCurrent(PDP_PORTS::ARM_PIVOT);
}

double PDP4237::GetArmExtend()
{
	return mPDP->GetCurrent(PDP_PORTS::ARM_EXTEND);
}

double PDP4237::GetPDPPort(int PDPPort)
{
	return mPDP->GetCurrent(PDPPort);
}



bool PDP4237::IsLeftDriveMasterOk()
{
	if(GetLeftDriveMaster() > PDP_PORTS::CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsLeftDriveFollowOk()
{
	if(GetLeftDriveFollow() > PDP_PORTS::CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsLeftShootMasterOk()
{
	if(GetLeftShootMaster() > PDP_PORTS::MINI_CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsLeftWinchMasterOk()
{
	if(GetLeftWinchMaster() > PDP_PORTS::MINI_CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsShooterLiftOk()
{
	if(GetShooterLift() > PDP_PORTS::MINI_CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsPusherOk()
{
	if(GetPusher() > PDP_PORTS::PLANETARY_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsLightRingPowerOk()
{
	if(GetLightRingPower() > PDP_PORTS::LIGHT_RING_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsRightDriveMasterOk()
{
	if(GetRightDriveMaster() > PDP_PORTS::CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsRightDriveFollowOk()
{
	if(GetRightDriveFollow() > PDP_PORTS::CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsRightShootMasterOk()
{
	if(GetRightShootMaster() > PDP_PORTS::MINI_CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsRightWinchMasterOk()
{
	if(GetRightWinchMaster() > PDP_PORTS::MINI_CIM_MAX_AMPS)
		return false;
	else
		return true;
}

bool PDP4237::IsArmPivotOk()
{
	if(GetArmPivot() > PDP_PORTS::WINDOW_MAX_AMPS)           //UPDATE if switch to BOSCH
		return false;
	else
		return true;
}

bool PDP4237::IsArmExtendOk()
{
	if(GetArmExtend() > PDP_PORTS::WINDOW_MAX_AMPS)          //UPDATE if switch to BOSCH
		return false;
	else
		return true;
}
