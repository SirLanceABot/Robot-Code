#ifndef LHS_VISION_H
#define LHS_VISION_H

/**
 * Sample program to use NIVision to find rectangles in the scene that are illuminated
 * by a LED ring light (similar to the model from FIRSTChoice). The camera sensitivity
 * is set very low so as to only show light sources and remove any distracting parts
 * of the image.
 * 
 * The CriteriaCollection is the set of criteria that is used to filter the set of
 * rectangles that are detected. In this example we're looking for rectangles with
 * a minimum width of 30 pixels and maximum of 400 pixels.
 * 
 * The algorithm first does a color threshold operation that only takes objects in the
 * scene that have a bright green color component. Then a small object filter
 * removes small particles that might be caused by green reflection scattered from other 
 * parts of the scene. Finally all particles are scored on rectangularity, and aspect ratio,
 * to determine if they are a target.
 *
 * Look in the VisionImages directory inside the project that is created for the sample
 * images.
 */

class LHSVision
{
public:
	//Structure to represent the scores for the various tests used for target identification
	enum TargetType{kNoTarget, kColdTarget, kHotLeftTarget, kHotRightTarget};

	struct Scores 
	{
		double rectangularity;
		double aspectRatioVertical;
		double aspectRatioHorizontal;
	};

	struct TargetReport 
	{
		int verticalIndex;
		int horizontalIndex;
		bool hot;
		double totalScore;
		double leftScore;
		double rightScore;
		double tapeWidthScore;
		double verticalScore;
		double distance;
	};

	LHSVision(void);
	~LHSVision(void);

	TargetType UpdateTarget(void);
	Scores* ScoreParticles(BinaryImage*, vector<ParticleAnalysisReport>*);
	TargetReport* FindBestTarget(BinaryImage*, vector<ParticleAnalysisReport>*);

	void SetCameraLight(bool);
	void CameraTest(void);

	bool FindTarget(void);
	bool GetHot(void);
	bool ScoreCompare(Scores, bool);
	bool HotOrNot(TargetReport*);

	double GetDistanceToTarget(void);
	double ComputeDistance(BinaryImage*, ParticleAnalysisReport*);
	double ScoreAspectRatio(BinaryImage*, ParticleAnalysisReport*, bool);
	double ScoreRectangularity(ParticleAnalysisReport*);
	double RatioToScore(double);

private:
	AxisCamera& mCamera;
	HSLImage* mImage;
	Relay* mCameraLight;
	Timer* mVisionTimer;
	Scores* mScores;
	ParticleFilterCriteria2* mCriteria;
	TargetReport* mTarget;

	int mVerticalTargets[MAX_PARTICLES];
	int mHorizontalTargets[MAX_PARTICLES];
	int mVerticalTargetCount;
	int mHorizontalTargetCount;
};

LHSVision::LHSVision(void) :
						mCamera(AxisCamera::GetInstance("10.42.37.11"))
{
	printf("File %s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	mCameraLight = new Relay(MODULE_1, LED_RING_RELAY, Relay::kForwardOnly);
	mVisionTimer = new Timer();
	mVisionTimer->Reset();

	//Particle filter criteria, used to filter out small particles	
	mCriteria = new ParticleFilterCriteria2();
	mCriteria->parameter = IMAQ_MT_AREA;
	mCriteria->lower = AREA_MINIMUM;
	mCriteria->upper = 65535;
	mCriteria->calibrated = false;
	mCriteria->exclude = false;

	printf("\n=====================================================================\n");
	printf("LHS Vision Constructor Started\n");

	while(!mCamera.IsFreshImage())
	{	
		printf("Waiting for camera thread initialization.\n");
		Wait(0.5);
	}

	printf("Camera initialized. Image is fresh.\n");

	mCamera.WriteResolution(AxisCameraParams::kResolution_320x240); 
	mCamera.WriteBrightness(BRIGHTNESS);
	mCamera.WriteMaxFPS(MAXFPS);

	mImage = NULL;
	mScores = NULL;

	mTarget = new TargetReport();
	mTarget->verticalIndex = 0;
	mTarget->horizontalIndex = 0;
	mTarget->hot = false;
	mTarget->totalScore = 0;
	mTarget->leftScore = 0;
	mTarget->rightScore = 0;
	mTarget->tapeWidthScore = 0;
	mTarget->verticalScore = 0;
	mTarget->distance = 0;

	for(int i = 0; i< MAX_PARTICLES; i++)
	{
		mVerticalTargets[i] = 0;
		mHorizontalTargets[i] = 0;
	}
	mVerticalTargetCount = 0;
	mHorizontalTargetCount = 0;

	//Prints
	printf("mCamera.GetBrightness= %d\n", mCamera.GetBrightness());
	printf("mCamera.GetColorLevel= %d\n", mCamera.GetColorLevel());
	printf("mCamera.GetCompression= %d\n", mCamera.GetCompression());
	printf("mCamera.GetExposureControl= %d\n", mCamera.GetExposureControl());
	printf("mCamera.GetExposurePriority= %d\n", mCamera.GetExposurePriority());
	printf("mCamera.GetMaxFPS= %d\n", mCamera.GetMaxFPS());
	printf("mCamera.GetResolution= %d\n", mCamera.GetResolution());
	printf("mCamera.GetRotation= %d\n", mCamera.GetRotation());
	printf("mCamera.GetWhiteBalance= %d\n", mCamera.GetWhiteBalance());

	printf("LHS Constructor Completed\n");
	printf("=====================================================================\n\n");
}

LHSVision::~LHSVision(void)
{
	mCamera.DeleteInstance();
	delete mCameraLight;
	mCameraLight = NULL;
	delete mVisionTimer;
	mVisionTimer = NULL;
	delete mCriteria;
	mCriteria = NULL;
}

LHSVision::TargetType LHSVision::UpdateTarget(void)
{
	if(FindTarget()) 
	{
		printf("Imaging completed time: %f\n", mVisionTimer->Get()); //time
		if (mTarget->verticalIndex == -999)
		{
			printf("No target\n");
			return kNoTarget;
		}
		else if(mTarget->hot)
		{
			if (mTarget->leftScore >= mTarget->rightScore)
			{
				printf("Hot target LEFT located\n");
				return kHotLeftTarget;
			}
			else
			{
				printf("Hot target RIGHT located\n");
				return kHotRightTarget;
			}
		}
		else
		{
			printf("Cold target located\n");
			return kColdTarget;
		}
	}
	else
	{
		return kNoTarget;
	}
}

void LHSVision::SetCameraLight(bool turnOn)
{
	if(turnOn)
	{
		mCameraLight->Set(Relay::kOn);
	}
	else
	{
		mCameraLight->Set(Relay::kOff);
	}
}

void LHSVision::CameraTest(void)
{
	mCameraLight->Set(Relay::kOn);  //LEDs on to illuminate target

	FindTarget();

	if(mTarget->verticalIndex == -999) 
	{
		printf("No target\n");
	}
	else if(mTarget->hot)
	{
		printf("Hot target located\n");
	}

	SmartDashboard::PutNumber("Distance", mTarget->distance);
	SmartDashboard::PutBoolean("Is target hot?", mTarget->hot);

	// done with the LED lights
	mCameraLight->Set(Relay::kOff);
}

bool LHSVision::FindTarget(void)
{
	if(mCamera.IsFreshImage())
	{
		mImage = mCamera.GetImage();
		//image->Write("/rawImage.bmp");

		BinaryImage* thresholdImage = mImage->ThresholdHSL((int)HMIN, (int)HMAX, (int)SMIN, (int)SMAX, (int)LMIN, (int)LMAX);
		//thresholdImage->Write("/threshold.bmp");

		BinaryImage* filteredImage = thresholdImage->ParticleFilter(mCriteria, 1);	//Remove small particles
		//filteredImage->Write("/Filtered.bmp");

		vector<ParticleAnalysisReport>* reports = filteredImage->GetOrderedParticleAnalysisReports();  //get a particle analysis report for each particle

		mVerticalTargetCount = mHorizontalTargetCount = 0;

		//Iterate through each particle, scoring it and determining whether it is a target or not
		//printf("Report Size=%d\n",reports->size());

		//target structure saves particle information of best candidate yet as particles are being processed
		//Zero out scores; assume target not Hot; set -999 for variables that shouldn't be used if not a target
		mTarget->totalScore = mTarget->leftScore = mTarget->rightScore = mTarget->tapeWidthScore = mTarget->verticalScore = 0;
		mTarget->hot = false;
		mTarget->distance = -999.;
		mTarget->verticalIndex = -999;
		mTarget->horizontalIndex = -999;

		if(reports->size() > 0)  // process any particles
		{
			mScores = new Scores[reports->size()];
			mScores = ScoreParticles(filteredImage, reports);
			mTarget = FindBestTarget(filteredImage, reports);

			// if at least one potential vertical target, compute distance to it
			// try to use best mTarget->totalScore target
			// if there wasn't one (no horizontal found for all the verticals) then use best vertical shape - most like the tape
			if (mVerticalTargetCount > 0)
			{
				if (mTarget->verticalIndex == -999) 
				{ // if no vertical target had a horizontal target, find best lonely vertical target
					mTarget->verticalIndex = mVerticalTargets[0]; // use first vertical target to get comparisions started

					mVisionTimer->Reset();
					mVisionTimer->Start();

					for (int i = 1; i < mVerticalTargetCount && mVisionTimer->Get() < 1; i++) // search vertical targets for best shape
					{
						if(mScores[mVerticalTargets[i]].aspectRatioVertical > mScores[mTarget->verticalIndex].aspectRatioVertical)
						{
							mTarget->verticalIndex = mVerticalTargets[i];
						}
					}
				}
				//printf("score for distance %f\n", scores[mTarget->verticalIndex].aspectRatioVertical);

				//Information about the target is contained in the "target" structure
				//To get measurement information such as sizes or locations use the
				//vertical index to get the particle report as shown below

				//make sure potential tape target is of high quality
				ParticleAnalysisReport* distanceReport = &(reports->at(mTarget->verticalIndex));

				mTarget->distance = ComputeDistance(filteredImage, distanceReport);
				//printf("\ntarget %d %d %c %f %f %f %f %f %f",mTarget->verticalIndex,mTarget->horizontalIndex,mTarget->Hot,
				//mTarget->totalScore,mTarget->leftScore,mTarget->rightScore,mTarget->tapeWidthScore,mTarget->verticalScore,mTarget->distance);
			}
			delete[] mScores;
			mScores = NULL;
		}
		else 
		{
			printf("\nNo particle\n");
		}

		delete filteredImage;
		filteredImage = NULL;
		delete thresholdImage;
		thresholdImage = NULL;
		delete reports;
		reports = NULL;
		delete mImage;
		mImage = NULL;

		return true;
	}
	else
	{
		return false;
	}
}

LHSVision::TargetReport* LHSVision::FindBestTarget(BinaryImage* filteredImage, vector<ParticleAnalysisReport>* reports)
{
	mVisionTimer->Reset();
	mVisionTimer->Start();
	// loop through all the potential vertical target particles since that target is static (if in the field of view)
	for (int i = 0; i < mVerticalTargetCount && mVisionTimer->Get() < 1; i++)
	{
		ParticleAnalysisReport* verticalReport = &(reports->at(mVerticalTargets[i]));

		// loop through all the potential horizontal particles to match up with the potential vertical one being processed
		// the horizontal target is dynamic so it might not be present even if the vertical one is
		for (int j = 0; j < mHorizontalTargetCount && mVisionTimer->Get() < 1; j++)
		{
			ParticleAnalysisReport* horizontalReport = &(reports->at(mHorizontalTargets[j]));
			double horizWidth, horizHeight, vertWidth, leftScore, rightScore, tapeWidthScore, verticalScore, total;

			//Measure equivalent rectangle sides for use in score calculation
			imaqMeasureParticle(filteredImage->GetImaqImage(), horizontalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &horizWidth);
			imaqMeasureParticle(filteredImage->GetImaqImage(), verticalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &vertWidth);
			imaqMeasureParticle(filteredImage->GetImaqImage(), horizontalReport->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &horizHeight);

			//Determine if the horizontal target is in the expected location to the left of the vertical target
			leftScore = RatioToScore(1.2*(verticalReport->boundingRect.left - horizontalReport->center_mass_x)/horizWidth);

			//Determine if the horizontal target is in the expected location to the right of the  vertical target
			rightScore = RatioToScore(1.2*(horizontalReport->center_mass_x - verticalReport->boundingRect.left - verticalReport->boundingRect.width)/horizWidth);

			//Determine if the width of the tape on the two targets appears to be the same
			tapeWidthScore = RatioToScore(vertWidth/horizHeight);

			//Determine if the vertical location of the horizontal target appears to be correct
			verticalScore = RatioToScore(1-(verticalReport->boundingRect.top - horizontalReport->center_mass_y)/(4*horizHeight));

			total = leftScore > rightScore ? leftScore:rightScore;
			total += tapeWidthScore + verticalScore;

			//If the target is the best detected so far store the information about it
			if(total > mTarget->totalScore)
			{
				mTarget->horizontalIndex = mHorizontalTargets[j];
				mTarget->verticalIndex = mVerticalTargets[i];
				mTarget->totalScore = total;
				mTarget->leftScore = leftScore;
				mTarget->rightScore = rightScore;
				mTarget->tapeWidthScore = tapeWidthScore;
				mTarget->verticalScore = verticalScore;
				mTarget->hot = HotOrNot(mTarget);
			}
		}
	}
	return mTarget;
}

LHSVision::Scores* LHSVision::ScoreParticles(BinaryImage* filteredImage, vector<ParticleAnalysisReport>* reports)
{
	mVisionTimer->Reset();
	mVisionTimer->Start();
	//rate and classify each particle as horizontal or vertical target or not a target
	for (unsigned int i = 0; i < MAX_PARTICLES && i < reports->size() && mVisionTimer->Get() < 1; i++)
	{
		ParticleAnalysisReport* report = &(reports->at(i));

		//Score each particle on rectangularity and aspect ratio
		mScores[i].rectangularity = ScoreRectangularity(report);
		mScores[i].aspectRatioVertical = ScoreAspectRatio(filteredImage, report, true);
		mScores[i].aspectRatioHorizontal = ScoreAspectRatio(filteredImage, report, false);			

		//Check if the particle is a horizontal target, if not, check if it's a vertical target
		if(ScoreCompare(mScores[i], false))
		{
			//printf("particle: %d  is a Horizontal Target centerX: %d  centerY: %d area: %f\n", i, report->center_mass_x, report->center_mass_y, report->particleArea);
			mHorizontalTargets[mHorizontalTargetCount++] = i; //Add particle to target array and increment count
		} 
		else if (ScoreCompare(mScores[i], true)) 
		{
			//printf("particle: %d  is a Vertical Target centerX: %d  centerY: %d area: %f\n", i, report->center_mass_x, report->center_mass_y, report->particleArea);
			mVerticalTargets[mVerticalTargetCount++] = i;  //Add particle to target array and increment count
		}

	}
	//else 
	//{
	//printf("particle: %d  is not a Target centerX: %d  centerY: %d area: %f\n", i, report->center_mass_x, report->center_mass_y, report->particleArea);
	//}
	//printf("Partical %d of %d: Scores rect: %f  ARvert: %f  ARhoriz: %f\n"
	//	,i ,reports->size(), scores[i].rectangularity, scores[i].aspectRatioVertical, scores[i].aspectRatioHorizontal);	
	return mScores;
}

bool LHSVision::GetHot(void)
{
	return mTarget->hot;
}

double LHSVision::GetDistanceToTarget(void)
{
	return mTarget->distance;
}

/**
 * Computes the estimated distance to a target using the height of the particle in the image. For more information and graphics
 * showing the math behind this approach see the Vision Processing section of the ScreenStepsLive documentation.
 * 
 * @param image The image to use for measuring the particle estimated rectangle
 * @param report The Particle Analysis Report for the particle
 * @return The estimated distance to the target in feet.
 */
double LHSVision::ComputeDistance (BinaryImage* image, ParticleAnalysisReport* report)
{
	double rectLong, height;
	int targetHeight;

	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
	//using the smaller of the estimated rectangle long side and the bounding rectangle height results in better performance
	//on skewed rectangles
	height = min(report->boundingRect.height, rectLong);
	targetHeight = 32; // vertical tape is 32 inches tall (4 inches wide)

	return (double)Y_IMAGE_RES * targetHeight / (height * 12 * 2 * tan((double)VIEW_ANGLE/2*((double)PI/180)));
}

/**
 * Computes a score (0-100) comparing the aspect ratio to the ideal aspect ratio for the target. This method uses
 * the equivalent rectangle sides to determine aspect ratio as it performs better as the target gets skewed by moving
 * to the left or right. The equivalent rectangle is the rectangle with sides x and y where particle area= x*y
 * and particle perimeter= 2x+2y
 * 
 * @param image The image containing the particle to score, needed to perform additional measurements
 * @param report The Particle Analysis Report for the particle, used for the width, height, and particle number
 * @param outer	Indicates whether the particle aspect ratio should be compared to the ratio for the inner target or the outer
 * @return The aspect ratio score (0-100)
 */
double LHSVision::ScoreAspectRatio(BinaryImage* image, ParticleAnalysisReport* report, bool vertical)
{
	double rectLong, rectShort, idealAspectRatio, aspectRatio;
	idealAspectRatio = vertical ? (4.0/32) : (23.5/4);	//Vertical reflector 4" wide x 32" tall, horizontal 23.5" wide x 4" tall

	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
	imaqMeasureParticle(image->GetImaqImage(), report->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &rectShort);

	//Divide width by height to measure aspect ratio
	if(report->boundingRect.width > report->boundingRect.height)
	{
		//particle is wider than it is tall, divide long by short
		aspectRatio = RatioToScore(((rectLong/rectShort)/idealAspectRatio));
	} 
	else 
	{
		//particle is taller than it is wide, divide short by long
		aspectRatio = RatioToScore(((rectShort/rectLong)/idealAspectRatio));
	}
	return aspectRatio;		//force to be in range 0-100
}

/**
 * Compares scores to defined limits and returns true if the particle appears to be a target
 * 
 * @param scores The structure containing the scores to compare
 * @param vertical True if the particle should be treated as a vertical target, false to treat it as a horizontal target
 * 
 * @return True if the particle meets all limits, false otherwise
 */
bool LHSVision::ScoreCompare(Scores scores, bool vertical)
{
	bool isTarget = true;

	isTarget &= scores.rectangularity > RECTANGULARITY_LIMIT;

	if(vertical)
	{
		isTarget &= scores.aspectRatioVertical > ASPECT_RATIO_LIMIT;
	} 
	else 
	{
		isTarget &= scores.aspectRatioHorizontal > ASPECT_RATIO_LIMIT;
	}

	return isTarget;
}

/**
 * Computes a score (0-100) estimating how rectangular the particle is by comparing the area of the particle
 * to the area of the bounding box surrounding it. A perfect rectangle would cover the entire bounding box.
 * 
 * @param report The Particle Analysis Report for the particle to score
 * @return The rectangularity score (0-100)
 */
double LHSVision::ScoreRectangularity(ParticleAnalysisReport *report)
{
	if(report->boundingRect.width*report->boundingRect.height !=0)
	{
		return 100*report->particleArea/(report->boundingRect.width*report->boundingRect.height);
	} 
	else 
	{
		return 0;
	}	
}	

/**
 * Converts a ratio with ideal value of 1 to a score. The resulting function is piecewise
 * linear going from (0,0) to (1,100) to (2,0) and is 0 for all inputs outside the range 0-2
 */
double LHSVision::RatioToScore(double ratio)
{
	return (max(0, min(100*(1-fabs(1-ratio)), 100)));
}

/**
 * Takes in a report on a target and compares the scores to the defined score limits to evaluate
 * if the target is a hot target or not.
 * 
 * Returns True if the target is hot. False if it is not.
 */
bool LHSVision::HotOrNot(TargetReport* target)
{
	bool isHot = true;

	isHot &= target->tapeWidthScore >= TAPE_WIDTH_LIMIT;
	isHot &= target->verticalScore >= VERTICAL_SCORE_LIMIT;
	isHot &= (target->leftScore > LR_SCORE_LIMIT) | (target->rightScore > LR_SCORE_LIMIT);

	return isHot;
}

#endif
