// Author: xXMarkShark99Xx
// Date: 2/17/2017
// Revision History:

#ifndef VISION_H
#define VISION_H

#include <vector>
#include <memory>
#include "Sonar.h"
#include "LightRing.h"
//#include "Constants.h"
#include "GripPipeline.h"
#include "cscore.h"

namespace frc
{
	class CameraServer;
	class Timer;
}

class NetworkTable;

class Vision
{
public:
	enum TurnDirection {kLeft = -1, kRight = 1, kStraight = 0};

	static constexpr int CENTER = 160; //Originally 160
	static constexpr int FINAL_ALIGNMENT_POINT = 250; //Originally 250
	//static constexpr int CLOSE_TO_TARGET_AREA = 1400;
	static constexpr int CLOSE_TO_TARGET_HEIGHT = 55;	//used to be 70, then 65 ( with camera to the left)
	static constexpr int CLOSE_TO_TARGET_DISTANCE_SONAR = 5;
	static constexpr int ACCEPTABLE_OFFSET = 6;

	Vision(int sonarPort);
	int GetMidpoint();
	int GetNumTargets();
	int GetOffset();
	int GetLargestHeight();

	bool IsCloseToTargetWithSensor();          //Checks if sonar/IR sensor is less than a certain distance away
	bool IsCloseToTargetWithVision();          //Checks if robot is close to peg using only vision
	bool IsAlignedWithTarget();                //Return true if robot is aligned with target
	double GetTurnSpeed();
	double GetTurnSpeedUsingCenter();
	std::vector<double> GetHeightVector();

	bool Update();
	void UpdateMidpoint();
	void UpdateNumTargets();
	void UpdateOffset();
	void UpdateLargestHeight();
	void UpdateTargetInfoVector();
	void UpdateCenterXVector();
	void UpdateHeightVector();
	void UpdateCurrentAlignmentPoint();
	void SortHeightCenterX();
	void IncreaseExposure();
	void DecreaseExposure();
	static void VisionThread();
//	void UpdateLightIntensity();

	bool AreVectorsValid();

	bool CheckPi();
	void ResetPiTimer();
	void StartPiTimer();
	void StartVisionThread();
	void StopVisionThread();
	void SetGearCamera();

private:
	std::shared_ptr<NetworkTable> mNetworkTable;
	std::shared_ptr<Sonar> mSonar;
	std::shared_ptr<LightRing> mLight;
	std::shared_ptr<frc::Timer> mUsePiTimer;
	cs::UsbCamera mAutoCamera;
	//cs::UsbCamera mGearCamera;
	cs::VideoSink mVisionServer;

	int mMidpointX; //Midpoint
	int mNumTargets;
	int mOffset;
	int mLargestHeight;
	int mCurrentAlignmentPoint;	//This starts out at the CENTER, and becomes FINAL_ALIGNMENT_POINT when it reaches the target

	std::vector<double> mHeightVector;
	std::vector<double> mCenterXVector;

	std::vector<double> mTargetInfo;
	int mTargetInfoID;
	int mPreviousTargetInfoID;

	bool mIsPiWorking;

};

#endif /* SRC_VISION_H_ */
