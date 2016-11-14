/*
 * Vision.cpp
 *
 *  Created on: Feb 15, 2016
 *      Author: Michael Conard
 */

#include "Vision.h"
#include "WPILib.h"
#include <math.h>

Vision::Vision():
mRoboRealm(NetworkTable::GetTable("RoboRealm")), //Construct network object within the scope of Robot
mImageWidth(mRoboRealm->GetNumber("IMAGE_WIDTH", 320)) //get image width
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);
}

Vision::~Vision()
{

}

float Vision::UpdateVision()
{
	// does not loop
	// returns float number of pixels from center of shooter aim.
	// returns 999. if not able to compute position.
	// note there is a bias correction between camera center and shooter center that may have to be adjusted.
	
	int blobCount = mRoboRealm->GetNumber("BLOB_COUNT", 0);
	// alternative - could wait for blob to appear; maybe add a timer to wait a second or 2
	// while (roboRealm->GetNumber("BLOB_COUNT", 0.L) < .5L) { Wait(.03); }; // wait for a blob to reappear
	
	float xPosition = mRoboRealm->GetNumber("COG_X", -1.0); //get center of gravity of vision target

	//printf("xPosition: %f", xPosition);

	if(xPosition == -1 || blobCount == 0)
	{
		printf("NetworkTable error or no target\n");
		return 999.f; //error
	}

	return mImageWidth/2.0 - xPosition + 6.0; // positive deviation means object too far left - turn to the left
												// add 6 pixels for bias between camera and shooter aim
}
