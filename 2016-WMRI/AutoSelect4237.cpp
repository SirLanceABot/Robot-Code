#include "AutoSelect4237.h"

#include <WPILib.h>
#include <math.h>
#include "Constants.h"

AutoSelect4237::AutoSelect4237()
{
	mNetworkTable = NetworkTable::GetTable("AutoSelect4237");
}

AutoSelect4237::~AutoSelect4237()
{

}

int AutoSelect4237::getPosition()
{
	position = mNetworkTable->GetNumber("Position", 6); //Get the position
	return position;
}

std::string AutoSelect4237::getObstacle()
{
	obstacle = mNetworkTable->GetString("Obstacle", "None");
	return obstacle;
}

float AutoSelect4237::getSpeed()
{
	speed = mNetworkTable->GetNumber("Speed 2016", 1.0);
	return speed;
}

float AutoSelect4237::getOrientation()
{
	orientation = mNetworkTable->GetNumber("Orientation 2016", 0.0);
	return orientation;
}

bool AutoSelect4237::getArmEnabled()
{
	bool enableArm = mNetworkTable->GetBoolean("EnableArm", false);
	return enableArm;
}

bool AutoSelect4237::endEarly()
{
	bool doEndEarly = mNetworkTable->GetBoolean("End_Init", false);
	return doEndEarly;
}
