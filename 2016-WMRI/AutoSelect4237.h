#include <WPILib.h>
#include <string.h>

#ifndef SRC_AUTOSELECT4237_H_
#define SRC_AUTOSELECT4237_H_

class AutoSelect4237 //A class made for reading the values from Mark's autonomous selector.
{
public:
	AutoSelect4237();
	~AutoSelect4237();

	int getPosition();
	std::string getObstacle();
	float getSpeed();
	float getOrientation();
	bool getArmEnabled();
	bool endEarly();

private:
	std::shared_ptr<NetworkTable> mNetworkTable;
	int position = 6;
	std::string obstacle = "None";
	float speed = 1.0;
	float orientation = 0.0;
	bool enableArm = false;
};

#endif /* SRC_AUTOSELECT4237_H_ */
