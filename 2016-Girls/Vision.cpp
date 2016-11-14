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

// TODO Fix the UpdateVision function
// if xPosition is -1 the distFromCenter is 161
// use the BLOB_COUNT to see if there are any blobs
// just return distFromCenter, don't check for threshold
float Vision::UpdateVision()
{
	float xPosition = mRoboRealm->GetNumber("COG_X", -1.0); //get center of gravity of vision target
	int blobCount = mRoboRealm->GetNumber("BLOB_COUNT", 0);
	//printf("xPosition: %f", xPosition);

	if(xPosition == -1 || blobCount == 0)
	{
		printf("NetworkTable error or no target\n");
		return 999; //error
	}

	return mImageWidth/2.0 + 6.0 - xPosition; //positive means object too far right, negative means too far left
}
