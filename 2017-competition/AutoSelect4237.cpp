/* Author: Mark Washington
 * Date: 2017
 * Revision History: MB, AS removed variables from private section and changed functions to uppercase letters
 */
#include "AutoSelect4237.h"

#include "WPILib.h"

AutoSelect4237::AutoSelect4237()
{
	std::cout << "[AutoSelect4237]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << std::endl;
	mNetworkTable = NetworkTable::GetTable("AutoSelect4237");
}

int AutoSelect4237::GetPosition2016()
{
	return mNetworkTable->GetNumber("Position", 6); //Get the position
}

std::string AutoSelect4237::GetObstacle2016()
{
	return mNetworkTable->GetString("Obstacle", "None");
}

double AutoSelect4237::GetSpeed2016()
{
	return mNetworkTable->GetNumber("Speed 2016", 1.0);
}

bool AutoSelect4237::GetArmEnabled2016()
{
	return mNetworkTable->GetBoolean("EnableArm", false);
}

std::string AutoSelect4237::GetGearPosition2017()
{
    return mNetworkTable->GetString("Gear Position 2017", "None");
}

std::string AutoSelect4237::GetBoilerPosition2017()
{
    return mNetworkTable->GetString("Boiler Position", "Left");
}

bool AutoSelect4237::GetAutoShooting2017()
{
    return mNetworkTable->GetBoolean("Auto Shooting", true);
}

bool AutoSelect4237::GetTestMode2017()
{
    return mNetworkTable->GetBoolean("Test Mode", false);
}

double AutoSelect4237::GetBatteryID2017()
{
    return mNetworkTable->GetNumber("Battery ID", 0);
}

double AutoSelect4237::GetHueLow2017()
{
	return mNetworkTable->GetNumber("Hue Low", 51.0);
}

double AutoSelect4237::GetHueHigh2017()
{
	return mNetworkTable->GetNumber("Hue High", 99.0);
}

double AutoSelect4237::GetSaturationLow2017()
{
	return mNetworkTable->GetNumber("Saturation Low", 198.0);
}

double AutoSelect4237::GetSaturationHigh2017()
{
	return mNetworkTable->GetNumber("Saturation High", 255.0);
}

double AutoSelect4237::GetLuminanceLow2017()
{
	return mNetworkTable->GetNumber("Luminance Low", 40.0);
}

double AutoSelect4237::GetLuminanceHigh2017()
{
	return mNetworkTable->GetNumber("Luminance High", 113.0);
}
