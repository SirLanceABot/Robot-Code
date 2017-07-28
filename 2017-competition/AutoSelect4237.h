/* Author: Mark Washington
 * Date: 2017
 * Revision History: MB, AS removed variables from private section and changed functions to uppercase letters
 */
#ifndef AUTOSELECT4237_H
#define AUTOSELECT4237_H

#include <memory>

class NetworkTable;

class AutoSelect4237 //A class made for reading the values from Mark's autonomous selector.
{
public:
	AutoSelect4237();

    //2016
	int GetPosition2016();
	std::string GetObstacle2016();
	double GetSpeed2016();
	bool GetArmEnabled2016();
	bool EndEarly2016();

    //2017
    std::string GetGearPosition2017();
    std::string GetBoilerPosition2017();
    bool GetAutoShooting2017();
    bool GetTestMode2017();
    double GetBatteryID2017();
    double GetHueLow2017();
    double GetHueHigh2017();
    double GetSaturationLow2017();
    double GetSaturationHigh2017();
    double GetLuminanceLow2017();
    double GetLuminanceHigh2017();
    
private:
    std::shared_ptr<NetworkTable> mNetworkTable;
};

#endif
