#include "Vision.h"
#include "WPILib.h"
#include <float.h>
//#include <GripPipeline.h>
//#include <cmath>

priority_mutex gMutex;
std::vector<double> gTargetInfo; //Global target info from RIO
bool gIsProcessingVision;


Vision::Vision(int sonarPort)://Vision constructor, accepts ports for left talon, right talon, and sonar
mSonar(std::make_shared<Sonar>(sonarPort)),
//mGripPipeline(std::make_shared<GripPipeline>()),
mUsePiTimer(std::make_shared<Timer>())
//mLight(std::make_shared<LightRing>(ROBORIO_DIGITAL_PORT::VISION_LIGHT_POWER, ROBORIO_DIGITAL_PORT::LIGHT_FOURS_DIGIT, ROBORIO_DIGITAL_PORT::LIGHT_TWOS_DIGIT, ROBORIO_DIGITAL_PORT::LIGHT_ONES_DIGIT))
{
	std::cout << "[Vision]  ";
	std::cout << __FILE__ << "  ";
	std::cout << __DATE__ << "  ";
	std::cout << __TIME__ << "  ";
	std::cout << this << '\n';
	//Create NetworkTable
	mNetworkTable = NetworkTable::GetTable("GRIP/vision");
	mMidpointX = -9999; //No midpoint found
	mNumTargets = -9999;
	mCurrentAlignmentPoint = -9999;
	mOffset = -9999;
	mLargestHeight = -9999;
	mHeightVector = {};
	mCenterXVector = {};
	mTargetInfo = {-1, -1};
	mPreviousTargetInfoID = -1;
	mTargetInfoID = 0;
	mIsPiWorking = true;
	mAutoCamera = CameraServer::GetInstance()->StartAutomaticCapture("AutoCamera", "/dev/v4l/by-id/usb-HD_Camera_Manufacturer_HD_USB_Camera-video-index0");
	//mGearCamera = CameraServer::GetInstance()->StartAutomaticCapture("GearCamera", "/dev/v4l/by-id/usb-Microsoft_Microsoft\xC2\xAE_LifeCam_HD-3000-video-index0");
	//mAutoCamera.SetResolution(320, 240);
	//mGearCamera.SetResolution(320, 240);
	//mAutoCamera.SetFPS(30);
	//mGearCamera.SetFPS(30);
	mVisionServer = CameraServer::GetInstance()->GetServer();
	mVisionServer.SetSource(mAutoCamera);


	std::cout << "Waiting for 8 seconds\n";
	Wait(8.0);
	std::cout << "Done waiting for 8 seconds\n";
	mUsePiTimer->Stop();
	mUsePiTimer->Reset();
}

// define cout << vector<type>
template<typename _Tp>
std::ostream& operator << (std::ostream& os, std::vector<_Tp>& v)
{
   os << "[";
   for (_Tp& ii : v)
    {
        os << " " << ii;
    }
    os << "]";

    return os;
}

//checks to see if vectors are same size
bool Vision::AreVectorsValid()
{
	return mHeightVector.size() == mCenterXVector.size() && mCenterXVector.size() > 0;
}


/**
 * Check if robot is close to target
 * @param distance Distance to stop at
 * @return True or false if robot is close to target
 */
bool Vision::IsCloseToTargetWithSensor() //Check if distance on sonar is less than a certain distance
{
	std::cout << "Sonar: " << mSonar->GetDistance() << '\n';
	return mSonar->GetDistance() < CLOSE_TO_TARGET_DISTANCE_SONAR;
}

bool Vision::IsCloseToTargetWithVision()
{
	return mLargestHeight >= CLOSE_TO_TARGET_HEIGHT;
}

/**
 * Check if robot is aligned with target
 * @param midpoint Point to use for alignment
 * @return True or false is robot is aligned with target
 */
bool Vision::IsAlignedWithTarget() //Check if robot is aligned with target
{
	return (mCurrentAlignmentPoint - ACCEPTABLE_OFFSET) <= mMidpointX &&  mMidpointX <= (mCurrentAlignmentPoint + ACCEPTABLE_OFFSET);
}

/**
 * Get speed to turn at depending on how far robot is offset from target
 * @param offset Robot's offset from the target
 * @return Speed to turn at
 */
double Vision::GetTurnSpeed() //Get the speed to turn at for smooth alignment
{ // FIXME: use voltage not %VBus
	double kP = -.00069L; //FIXME:  The P of PID for auto turning by camera error -%VBus/delta pixel (inverted control)
	double controllerMin = .22L; // FIXME: lowest (absolute) value to move wheels put in constants.h watch those squared inputs which should be off
	double controllerMax = .7L; // highest (absolute )value

	double speed = 0.0;

	//  mOffset is PID error term

	if (mMidpointX >= 0) // verify there is a target (mOffset is valid)
	{
		if (abs(mOffset) <= ACCEPTABLE_OFFSET) // If within allowable offset, no control needed, stop turning
		{
			speed = 0.0;
		}
		else // In the proportional band
		{
			speed = kP * mOffset; // The P correction of the PID controller
			if (speed >= 0)
			{
				speed = std::min(speed,  controllerMax); // clamp at maximum speed allowed
				speed = std::max(speed,  controllerMin); // boost to minimum speed to move
			}
			else
			{
				speed = std::max(speed, -controllerMax); // clamp at maximum speed allowed
				speed = std::min(speed, -controllerMin); // boost to minimum speed to move
			}
		}
	}
	else // no valid target so don't turn
	{
		std::cout << "[Vision::GetTurnSpeed() " << __LINE__ << "] No valid target to turn toward" << '\n';
		speed = 0.0;
	}

	std::cout << "[Vision::GetTurnSpeed() " << __LINE__ << "] Turning Speed: " << speed << '\n';

	return speed;
}

/**
 * Get speed to turn at depending on how far robot is offset from target
 * @param offset Robot's offset from the target
 * @return Speed to turn at
 */
double Vision::GetTurnSpeedUsingCenter() // Turn slowly to find other contour
{ // if dead center then pick a direction to try (left - it's arbitrary)
	double controllerMin = .22L; // FIXME: lowest (absolute) value to move wheels  put in constants.h watch those squared inputs which should be turned off
	TurnDirection turnDirection;
	double speed;

	if (mMidpointX > CENTER)
	{
		turnDirection = kRight;
	}
	else
	{
		turnDirection = kLeft;
	}

	speed =  controllerMin * turnDirection; // FIXME: lowest (absolute) value to move wheels

	std::cout << "GetTurnSpeedUsingCenter Turning Speed: " << speed << '\n';

	return speed;
}

/**
 * Returns number of targets found in the camera's view
 * @return Number of targets found
 */
int Vision::GetNumTargets() //Get number of targets found
{
	return mNumTargets;
}

/**
 * Method to get midpoint between two target strips
 * @return Midpoint
 */
int Vision::GetMidpoint() //Get midpoint of targets
{
	return mMidpointX;
}

/**
 * Get robot's offset from the center of the target
 * @return Offset
 */
int Vision::GetOffset() //Get offset from center
{
	return mOffset;
}

int Vision::GetLargestHeight()
{
	return mLargestHeight;
}

std::vector<double> Vision::GetHeightVector()
{
	return mHeightVector;
}

void Vision::UpdateTargetInfoVector()
{
	//if (true)
	if (mIsPiWorking)
	{
		mTargetInfo = mNetworkTable->GetNumberArray("Pi TargetInfo", -9999);
	}
	else
	{
		//mCVSink.GrabFrame(mMat);
		//mGripPipeline->process(mMat);
		{
			std::lock_guard<priority_mutex> protectMe(gMutex);
			mTargetInfo = gTargetInfo;
		}
	}
	std::cout << "TargetInfo " << mTargetInfo << '\n';
	mCenterXVector.clear();
	mHeightVector.clear();
	/*
	 *  verify TargetInfo vector and extract data
	 */
	// TargetInfo vector:
	// contour 1: cog X, area,
	// contour 2: cog X, area,
	// ...
	// frame id, checksum (sum of all previous data)

	if (mTargetInfo.size() < 2 || mTargetInfo.size() % 2 != 0)  // bad error so skip processing this frame
	{
		std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Damaged camera frame, size: " << mTargetInfo.size() << '\n';
	}
	else // so far so good so keep checking and processing frame
	{
		double checksum = mTargetInfo.back(); // get the expected checksum
		mTargetInfoID = mTargetInfo[mTargetInfo.size() - 2]; // get the frame id
		checksum -= mTargetInfoID; // take off the frame id from the checksum

		// FIXME: clear COG X and area vectors here

		for (unsigned int targetInfoIndex = 0; targetInfoIndex < mTargetInfo.size() - 2; targetInfoIndex += 2)
		{
			checksum -= mTargetInfo[targetInfoIndex];
			// FIXME: extract COG X here
			mCenterXVector.push_back(mTargetInfo[targetInfoIndex]);

			checksum -= mTargetInfo[targetInfoIndex + 1];
			// FIXME: extract area here
			mHeightVector.push_back(mTargetInfo[targetInfoIndex + 1]);
		}

		if (checksum != 0) // bad error so skip processing this frame
		{
			mCenterXVector.clear();
			mHeightVector.clear();
			std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Camera frame error, checksum: " << checksum << '\n';
		}
		else // fairly benign errors or good frame so process this frame
		{
			if (mPreviousTargetInfoID == mTargetInfoID) // skip duplicates
			{
				//mCenterXVector.clear();
				//mHeightVector.clear();
				std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Duplicate camera frame " << mTargetInfoID << '\n';
			}
			else // new unique data so process it
			{
				// see if any intervening frames were skipped
				if (mPreviousTargetInfoID + 1 != mTargetInfoID)
				{
					std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Skipped processing camera frame " << mPreviousTargetInfoID + 1 << " to frame " << mTargetInfoID - 1 << '\n';
				}

				// FIXME: process this target data
				std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Processing frame id " << mTargetInfoID << '\n';

				//std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Center X Vector " << mCenterXVector << '\n';
				//std::cout << "[Vision::UpdateTargetInfoVector() " << __LINE__ << "] Area Vector " << mHeightVector << '\n';
			}
		}
		if (mIsPiWorking)
		{
			mIsPiWorking = CheckPi();
		}
		mPreviousTargetInfoID = mTargetInfoID; // update last frame id processed
	}
}

void Vision::UpdateCenterXVector()
{
	mCenterXVector = mNetworkTable->GetNumberArray("centerX", -9999);
}

void Vision::UpdateHeightVector()
{
	mHeightVector = mNetworkTable->GetNumberArray("height", -9999);
}

//Sorts area and centerX vectors by area size descending order
void Vision::SortHeightCenterX()
{
	unsigned int i, j;
	for(i = 0; i < mHeightVector.size(); i++)
	{
		for(j = i + 1; j < mHeightVector.size(); j++)
		{
			if(mHeightVector[i] < mHeightVector[j])
			{
				std::swap(mHeightVector[i], mHeightVector[j]);
				std::swap(mCenterXVector[i], mCenterXVector[j]);
			}
		}
	}
	std::cout << "[Vision::SortHeightCenterX() " << __LINE__ << "] Processing frame id " << mTargetInfoID << '\n';
	std::cout << "Height Vector " << mHeightVector << '\n';
	std::cout << "Center X Vector " << mCenterXVector << '\n';
}

void Vision::UpdateNumTargets() //Gets
{
	mNumTargets = mCenterXVector.size();
	if(mNumTargets > 2)
	{
		std::cout << "[Vision::UpdateNumTargets() " << __LINE__ << "] Number of contours found: " << mNumTargets << '\n';
		//std::cout << "[Vision::UpdateNumTargets() " << __LINE__ << "] Center x is " << mCenterXVector << '\n';
		mNumTargets = 2; // using first 2 (largest) targets
	}
}

void Vision::UpdateMidpoint()
{
//	if (mNumTargets >= 2)
//	{
//		int mid = (int)((mCenterXVector[0] + mCenterXVector[1]) / 2);
//		mMidpointX = mid;
//	}
//	else if (mNumTargets == 1)
//	{
//		mMidpointX = mCenterXVector[0];
//	}
//	else
//	{
//		mMidpointX = -9999; //Error
//	}

	if (mNumTargets >= 2)
	{
		if(mCenterXVector[0] > mCenterXVector[1])
		{
			mMidpointX = mCenterXVector[1];
		}
		else
		{
			mMidpointX = mCenterXVector[0];
		}
	}
	else if (mNumTargets == 1)
	{
		mMidpointX = mCenterXVector[0];
	}
	else
	{
		mMidpointX = -9999; //Error
	}
	std::cout << "[Vision::UpdateMidpoint() " << __LINE__ << "] MidpointX: " << mMidpointX << '\n';

	if(mNumTargets >= 2 && mCenterXVector[0] > mCenterXVector[1])
	{
		std::cout << "WARNING [Vision::UpdateMidpoint() " << __LINE__ << "] Seeing two targets and left is smaller" << '\n';
	}

}

void Vision::UpdateLargestHeight()
{
	if (mNumTargets > 0)
	{
		mLargestHeight = mHeightVector[0];
	}
	else
	{
		mLargestHeight = CLOSE_TO_TARGET_HEIGHT;
	}
	std::cout << "[Vision::UpdateLargestHeight() " << __LINE__ << "] Max height: " << mLargestHeight << '\n';
}

//changes light intensity based on area of largest vector
//void Vision::UpdateLightIntensity()
//{
//	if(mLargestHeight >= CLOSE_TO_TARGET_AREA)
//	{
//		mLight->SetIntensity(LightRing::k100v);
//	}
//	else if(mLargestHeight > 1200)
//	{
//		mLight->SetIntensity(LightRing::k116v);
//	}
//	else if(mLargestHeight > 1000)
//	{
//		mLight->SetIntensity(LightRing::k132v);
//	}
//}

void Vision::UpdateCurrentAlignmentPoint()
{
	// mCurrentAlignmentPoint = (double)CENTER + (double)(FINAL_ALIGNMENT_POINT - CENTER) * (sqrt((double)mLargestHeight / (double)CLOSE_TO_TARGET_AREA));
//	mCurrentAlignmentPoint = (double)CENTER + (double)(FINAL_ALIGNMENT_POINT - CENTER) * std::min((double)mLargestHeight / (double)CLOSE_TO_TARGET_AREA, 1.0);
	mCurrentAlignmentPoint = (double)CENTER + (double)(110 - CENTER) * std::min((double)mLargestHeight / (double)CLOSE_TO_TARGET_HEIGHT, 1.0);
	std::cout << "[Vision::UpdateCurrentAlignmentPoint() " << __LINE__ << "] Current Alignment point: " << mCurrentAlignmentPoint << '\n';
}

void Vision::UpdateOffset()
{
	mOffset = mCurrentAlignmentPoint - mMidpointX;
	std::cout << "[Vision::UpdateOffset() " << __LINE__ << "] Offset: " << mOffset << '\n';
}

bool Vision::Update()
{
	bool isDataGood;
	//UpdateCenterXVector();     //Uses NetworkTables
	//UpdateAreaVector();       //Uses NetworkTables
	UpdateTargetInfoVector();
	isDataGood = AreVectorsValid();
	if(isDataGood == true)
	{
		SortHeightCenterX();	//Sorts both vectors in descending order
		UpdateNumTargets();      //Relies on UpdateCenterXVector (results in 0, 1, or 2 targets)
		UpdateMidpoint();       //Relies on UpdateCenterXVector (results in -9999 if no target, or the center or 1 or the pair of targets)
		UpdateLargestHeight();  //Relies on UpdateAreaVector & SortAreaVector & UpdateNumTargets
		//UpdateLightIntensity();
		UpdateCurrentAlignmentPoint(); //Relies on UpdateLargestArea
		UpdateOffset();        //Relies on UpdateMidpoint and UpdateCurrentAlignmentPoint (results in error between desired and center of 1 or a pair of targets)
	}
	else
	{
		std::cout << "ERROR [Vision::Update() " << __LINE__ << "] The vectors are either empty or not the same size" << '\n';
		std::cout << "Height Vector " << mHeightVector << '\n';
		std::cout << "Center X Vector " << mCenterXVector << '\n';
	}

	return isDataGood;
}

bool Vision::CheckPi()
{
	double time = mUsePiTimer->Get();
	std::cout << "Pi Timer: " << time << std::endl;
	if (mIsPiWorking == false)
	{
		std::cout << "WARNING Pi is not working\n";
		return false;
	}
	else if (mTargetInfo.size() < 2 || mTargetInfo.size() % 2 != 0)
	{
		if(time < 0.8)
		{
			std::cout << "WARNING Pi might not be working\n";
			return true;
		}
		else
		{
			std::cout << "WARNING Pi is not working switching to RIO\n";
			return false;
		}
	}
	else if (mPreviousTargetInfoID == mTargetInfoID)
	{
		if(time < 0.8)
		{
			std::cout << "WARNING Pi might not be working\n";
			return true;
		}
		else
		{
			std::cout << "WARNING Pi is not working switching to RIO\n";
			return false;
		}
	}
	else
	{
		//std::cout << "WARNING Pi is working\n";
		mUsePiTimer->Reset();
		return true;
	}
}

void Vision::IncreaseExposure()
{
	mAutoCamera.SetExposureManual(13);
}

void Vision::DecreaseExposure()
{
	mAutoCamera.SetExposureManual(0);
}

void Vision::VisionThread()
{
	std::cout << "Starting Vision Thread\n" << std::endl;
	cv::Mat mat;
	cs::CvSink CVSink = CameraServer::GetInstance()->GetVideo();
	Wait(1.0);
	std::shared_ptr<GripPipeline> gripPipeline = std::make_shared<GripPipeline>();

	while (gIsProcessingVision)
	{
		if (CVSink.GrabFrame(mat) == 0) continue;
		try
		{
			gripPipeline->process(mat);
			{
				std::lock_guard<priority_mutex> protectMe(gMutex);
				gTargetInfo = gripPipeline->extraProcessing();
			}
		}
		catch (cv::Exception& e)
		{
			std::cout << "An error occurred in the vision thread: " << e.msg << std::endl;
		}

		Wait(0.005);
	}
}

void Vision::ResetPiTimer()
{
	gIsProcessingVision = true;
	mIsPiWorking = true;
	mUsePiTimer->Stop();
	mUsePiTimer->Reset();
}

void Vision::StartPiTimer()
{
	mUsePiTimer->Start();
}

//starts vision thread
void Vision::StartVisionThread()
{
	gIsProcessingVision = true;
	std::thread mVisionThread(VisionThread);
	mVisionThread.detach();
}

//stops the vision thread
void Vision::StopVisionThread()
{
	gIsProcessingVision = false;
}

void Vision::SetGearCamera()
{
	//Wait(0.25);
	//mVisionServer.SetSource(mGearCamera);
	//mAutoCamera.SetResolution(80, 60);
	//mAutoCamera.SetExposureManual(0);
	//mAutoCamera.SetPixelFormat(cs::VideoMode::kGray);
	//CameraServer::GetInstance()->RemoveCamera("AutoCamera");
	//mGearCamera.SetExposureManual(70);
}
