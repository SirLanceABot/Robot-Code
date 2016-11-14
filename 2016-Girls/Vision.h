/*
 * Vision.h
 *
 *  Created on: Feb 15, 2016
 *      Author: Michael Conard
 */

#ifndef VISION_H_
#define VISION_H_

#include <memory>

class NetworkTable;

class Vision
{
public:
	Vision();
	~Vision();
	float UpdateVision();
private:
	std::shared_ptr<NetworkTable> mRoboRealm; //Network table object, communicate with RoboRealm
	float mImageWidth;
};

#endif /* SRC_VISION_H_ */
