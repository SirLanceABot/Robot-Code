#ifndef LHS_VISION_H
#define LHS_VISION_H

class LHSVision
{
public:
	struct ParticleReportTape 	// for tape
	{
		double PercentAreaToImageArea;
		double Area;
		double BoundingRectLeft;
		double BoundingRectTop;
		double BoundingRectRight;
		double BoundingRectBottom;
		double CenterOfXMass;
	};

	struct ScoresTape 	// for tape
	{
		double Area;
		double Aspect;
	};

	struct ParticleReportTote  // for tote
	{
		double PercentAreaToImageArea;
		double Area;
		double ConvexHullArea;
		double BoundingRectLeft;
		double BoundingRectTop;
		double BoundingRectRight;
		double BoundingRectBottom;
	};

	struct ScoresTote   // for tote
	{
		double Trapezoid;
		double LongAspect;
		double ShortAspect;
		double AreaToConvexHullArea;
	};

	LHSVision(RobotDrive*, LHSJoystick*);
	~LHSVision();
	void UseCameraForTape(); // for tape
	void UseCameraForTote(); // for tote
	void SendToDashboard(Image*, int); // for both
	double RatioToScore(double);  // for both
	double AreaScore(ParticleReportTape);  // for tape
	double AspectScore(ParticleReportTape);  // for tape
	double ComputeDistanceTape(Image*, ParticleReportTape);  // for tape
	double ComputeDistanceTote(Image*, ParticleReportTote, bool); // for tote
	double ConvexHullAreaScore(ParticleReportTote); // for tote
	double TrapezoidScore(ParticleReportTote); // for tote
	double LongSideScore(ParticleReportTote); // for tote
	double ShortSideScore(ParticleReportTote); // for tote
	void AdjustToTape(std::vector<ParticleReportTape>); // for tape, by Michael
	void TurnLightOff();
	bool TestVision();
	void TurnLightOn();
	void StopCamera(int);
	void StartCamera(int);

	//Comparator function for sorting particles. Returns true if particle 1 is larger
	static bool CompareParticleSizesTape(ParticleReportTape particle1, ParticleReportTape particle2) // for tape
	{
		//we want descending sort order
		return particle1.PercentAreaToImageArea > particle2.PercentAreaToImageArea;
	}

	//Comparator function for sorting particles. Returns true if particle 1 is larger
	static bool CompareParticleSizesTote(ParticleReportTote particle1, ParticleReportTote particle2) // for tote
	{
		//we want descending sort order
		return particle1.PercentAreaToImageArea > particle2.PercentAreaToImageArea;
	}

private:
	// for both
	IMAQdxSession session;
	Image *frame;
	Image *binaryFrame;

	IMAQdxSession session2;
	Image *frame2;
	Image *binaryFrame2;

	int imaqError;
	Relay* mLight;

	//Constants for tape
	Range RING_HUE_RANGE = {55, 100};	//Default hue range for ring light
	Range RING_SAT_RANGE = {100, 200};	//Default saturation range for ring light
	Range RING_VAL_RANGE = {200, 255};	//Default value range for ring light
	ScoresTape scoresTape;

	//Constants for tote
	Range TOTE_HUE_RANGE = {24, 49};	//Default hue range for yellow tote
	Range TOTE_SAT_RANGE = {130, 200};	//Default saturation range for yellow tote
	Range TOTE_VAL_RANGE = {160, 230};	//Default value range for yellow tote
	ScoresTote scoresTote;

	ParticleFilterCriteria2 criteria[1];
	ParticleFilterOptions2 filterOptions = {0,0,1,1};

	RobotDrive* mRobot;
	LHSJoystick* mXbox;

	int send = 1;
	int visionType = 3;
};

LHSVision::LHSVision(RobotDrive* pRobot, LHSJoystick* pXbox)
{
	printf("File %18s Date %s Time %s Object %p\n",__FILE__,__DATE__, __TIME__, this);

	frame = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	binaryFrame = imaqCreateImage(IMAQ_IMAGE_U8, 0);
	imaqError = IMAQdxOpenCamera("cam0", IMAQdxCameraControlModeController, &session); //Drop6 "cam1" Mecanum "cam0"
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);
	Wait(.5);
	imaqError = IMAQdxConfigureGrab(session);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);
	Wait(.5);
	IMAQdxStartAcquisition(session);
	Wait(.5);

	frame2 = imaqCreateImage(IMAQ_IMAGE_RGB, 0);
	binaryFrame2 = imaqCreateImage(IMAQ_IMAGE_U8, 0);

	mRobot = pRobot;
	mXbox = pXbox;
	mLight = new Relay(RING_LIGHT_RELAY, Relay::kForwardOnly);
}
LHSVision::~LHSVision()
{
	delete mXbox;
	mXbox = NULL;
	delete mRobot;
	mRobot = NULL;
	delete mLight;
	mLight = NULL;
}

void LHSVision::TurnLightOff()
{
	mLight->Set(Relay::kOff);
}

void LHSVision::TurnLightOn()
{
	mLight->Set(Relay::kOn);
}

void LHSVision::StopCamera(int camNum)
{
	if(camNum == 1)
	{
		IMAQdxStopAcquisition(session);
		Wait(.01);
		IMAQdxCloseCamera(session);
	}
	else
	{
		IMAQdxStopAcquisition(session2);
		Wait(.01);
		IMAQdxCloseCamera(session2);
	}
}

void LHSVision::StartCamera(int camNum)
{
	if(camNum == 1)
	{
		IMAQdxOpenCamera("cam0", IMAQdxCameraControlModeController, &session);
		Wait(.01);
		IMAQdxConfigureGrab(session);
		Wait(.01);
		IMAQdxStartAcquisition(session);
		Wait(.01);
	}
	else
	{
		IMAQdxOpenCamera("cam1", IMAQdxCameraControlModeController, &session2);
		Wait(.01);
		IMAQdxConfigureGrab(session2);
		Wait(.01);
		IMAQdxStartAcquisition(session2);
		Wait(.01);
	}
}

void LHSVision::UseCameraForTote()
{
	mLight->Set(Relay::kOff);

	imaqError = IMAQdxGrab(session, frame, true, NULL);
	Wait(.01);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	//Threshold the image looking for yellow (tote color)
	imaqError = imaqColorThreshold(binaryFrame, frame, 255, IMAQ_HSV, &TOTE_HUE_RANGE, &TOTE_SAT_RANGE, &TOTE_VAL_RANGE);

	int numParticles = 0;
	imaqError = imaqCountParticles(binaryFrame, 1, &numParticles);
	//printf("imaqError %d: %d\n", __LINE__, imaqError);
	//SmartDashboard::PutNumber("Masked particles", numParticles);

	SendToDashboard(binaryFrame, imaqError); //binaryFrame

	//filter out small particles
	float areaMin = AREA_MINIMUM_TOTE;//AREA_MINIMUM_TOTE; //SmartDashboard::GetNumber("Area min tote %", AREA_MINIMUM_TOTE);
	criteria[0] = {IMAQ_MT_AREA_BY_IMAGE_AREA, areaMin, 100, false, false};
	imaqError = imaqParticleFilter4(binaryFrame, binaryFrame, criteria, 1, &filterOptions, NULL, NULL);
	//printf("imaqError %d: %d\n", __LINE__, imaqError);
	imaqError = imaqCountParticles(binaryFrame, 1, &numParticles);
	//SmartDashboard::PutNumber("Filtered particles", numParticles);

	if(numParticles > 0)
	{
		//Measure particles and sort by particle size
		std::vector<ParticleReportTote> particles;
		for(int particleIndex = 0; particleIndex < numParticles; particleIndex++)
		{
			ParticleReportTote par;
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_AREA_BY_IMAGE_AREA, &(par.PercentAreaToImageArea));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_AREA, &(par.Area));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_CONVEX_HULL_AREA, &(par.ConvexHullArea));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_TOP, &(par.BoundingRectTop));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_LEFT, &(par.BoundingRectLeft));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_BOTTOM, &(par.BoundingRectBottom));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_RIGHT, &(par.BoundingRectRight));
			particles.push_back(par);
		}
		sort(particles.begin(), particles.end(), CompareParticleSizesTote);

		scoresTote.Trapezoid = TrapezoidScore(particles.at(0));
		//SmartDashboard::PutNumber("Trapezoid", scoresTote.Trapezoid);
		scoresTote.LongAspect = LongSideScore(particles.at(0));
		//SmartDashboard::PutNumber("Long Aspect", scoresTote.LongAspect);
		scoresTote.ShortAspect = ShortSideScore(particles.at(0));
		//SmartDashboard::PutNumber("Short Aspect", scoresTote.ShortAspect);
		scoresTote.AreaToConvexHullArea = ConvexHullAreaScore(particles.at(0));
		//SmartDashboard::PutNumber("Convex Hull Area", scoresTote.AreaToConvexHullArea);
		bool isTote = scoresTote.Trapezoid > SCORE_MIN && (scoresTote.LongAspect > SCORE_MIN || scoresTote.ShortAspect > SCORE_MIN) && scoresTote.AreaToConvexHullArea > SCORE_MIN;
		bool isLong = scoresTote.LongAspect > scoresTote.ShortAspect;

		//Send distance and tote status to dashboard. The bounding rect, particularly the horizontal center (left - right) may be useful for rotating/driving towards a tote
		SmartDashboard::PutBoolean("IsTote", isTote);
		SmartDashboard::PutBoolean("IsTarget", false);
		DBLED(0,true);
		SmartDashboard::PutNumber("Distance to tote", ComputeDistanceTote(binaryFrame, particles.at(0), isLong));
		SmartDashboard::PutNumber("Distance to tape", 0);
	}
	else
	{
		SmartDashboard::PutBoolean("IsTote", false);
		DBLED(0,false);
	}
	Wait(0.02);
}

void LHSVision::UseCameraForTape()
{
	mLight->Set(Relay::kOn);
	imaqError = IMAQdxGrab(session, frame, true, NULL);
	//SendToDashboard(frame, imaqError);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	//Threshold the image looking for ring light color
	imaqError = imaqColorThreshold(binaryFrame, frame, 255, IMAQ_HSV, &RING_HUE_RANGE, &RING_SAT_RANGE, &RING_VAL_RANGE);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	//Send particle count to dashboard
	int numParticles = 0;
	imaqError = imaqCountParticles(binaryFrame, 1, &numParticles);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	//SmartDashboard::PutNumber("Masked particles", numParticles);

	//Send masked image to dashboard to assist in tweaking mask.
	SendToDashboard(binaryFrame, imaqError);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	//filter out small particles
	float areaMin = AREA_MINIMUM_TAPE;//SmartDashboard::GetNumber("Area min tape %", AREA_MINIMUM_TAPE);
	criteria[0] = {IMAQ_MT_AREA_BY_IMAGE_AREA, areaMin, 100, false, false};
	imaqError = imaqParticleFilter4(binaryFrame, binaryFrame, criteria, 1, &filterOptions, NULL, NULL);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	//Send particle count after filtering to dashboard
	imaqError = imaqCountParticles(binaryFrame, 1, &numParticles);
	SmartDashboard::PutNumber("Filtered particles", numParticles);
	//printf("imaqError %d: %4x\n", __LINE__, imaqError);

	if(numParticles > 0) {
		//printf("imaqError %d: %d\n", __LINE__, numParticles);
		//Measure particles and sort by particle size
		std::vector<ParticleReportTape> particles;
		for(int particleIndex = 0; particleIndex < numParticles; particleIndex++)
		{
			ParticleReportTape par;
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_AREA_BY_IMAGE_AREA, &(par.PercentAreaToImageArea));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_AREA, &(par.Area));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_TOP, &(par.BoundingRectTop));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_LEFT, &(par.BoundingRectLeft));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_BOTTOM, &(par.BoundingRectBottom));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_BOUNDING_RECT_RIGHT, &(par.BoundingRectRight));
			imaqMeasureParticle(binaryFrame, particleIndex, 0, IMAQ_MT_CENTER_OF_MASS_X, &(par.CenterOfXMass));
			particles.push_back(par);
		}
		sort(particles.begin(), particles.end(), CompareParticleSizesTape);

		//AdjustToTape(particles);

		scoresTape.Aspect = AspectScore(particles.at(0));
		//SmartDashboard::PutNumber("Aspect", scoresTape.Aspect);
		scoresTape.Area = AreaScore(particles.at(0));
		//SmartDashboard::PutNumber("Area", scoresTape.Area);
		bool isTarget = scoresTape.Area > SCORE_MIN && scoresTape.Aspect > SCORE_MIN;

		SmartDashboard::PutBoolean("IsTarget", isTarget);
		SmartDashboard::PutBoolean("IsTote", false);
		DBLED(1,true);
		SmartDashboard::PutNumber("Distance to tape", ComputeDistanceTape(binaryFrame, particles.at(0)));
		SmartDashboard::PutNumber("Distance to tote", 0);
	}
	else
	{
		SmartDashboard::PutBoolean("IsTarget", false);
		DBLED(1,false);
	}

	Wait(0.07);
	//	mRobot->MecanumDrive_Cartesian(0.0, 0.0, 0.0); //->ArcadeDrive(0.0,0.0,true);
}

void LHSVision::AdjustToTape(std::vector<ParticleReportTape> particles) // for tape
{
	if(particles.size() > 1)
	{
		double middle = (particles.at(0).BoundingRectLeft + particles.at(1).BoundingRectRight)/2.0;

		printf("Particle 0: CenterOfXMass: %3.1f, Top: %3.1f, Bottom: %3.1f, Left: %3.1f, Right: %3.1f, Middle: %3.1f\n",particles.at(0).CenterOfXMass,
				particles.at(0).BoundingRectTop,particles.at(0).BoundingRectBottom,particles.at(0).BoundingRectLeft,particles.at(0).BoundingRectRight,middle);
		printf("Particle 1: CenterOfXMass: %3.1f, Top: %3.1f, Bottom: %3.1f, Left: %3.1f, Right: %3.1f, Middle: %3.1f\n",particles.at(1).CenterOfXMass,
				particles.at(1).BoundingRectTop,particles.at(1).BoundingRectBottom,particles.at(1).BoundingRectLeft,particles.at(1).BoundingRectRight,middle);

		if(middle >= 320.0 + 40.0)
			mRobot->MecanumDrive_Cartesian(0.0, 0.0, 0.20); //->ArcadeDrive(0.0,0.3,true);
		else if(middle <= 320.0 - 40.0)
			mRobot->MecanumDrive_Cartesian(0.0, 0.0, -0.20); //->ArcadeDrive(0.0,-0.3,true);
		else
			mRobot->MecanumDrive_Cartesian(0.0, 0.0, 0.0); //->ArcadeDrive(0.0,0.0,true);
	}
	else if(particles.size() > 0)
	{
		printf("Particle 0: CenterOfXMass: %3.1f, Top: %3.1f, Bottom: %3.1f, Left: %3.1f, Right: %3.1f\n",particles.at(0).CenterOfXMass,
				particles.at(0).BoundingRectTop,particles.at(0).BoundingRectBottom,particles.at(0).BoundingRectLeft,particles.at(0).BoundingRectRight);
		mRobot->MecanumDrive_Cartesian(0.0, 0.0, 0.0); //->ArcadeDrive(0.0,0.0,true);
	}
	else
	{
		mRobot->MecanumDrive_Cartesian(0.0, 0.0, 0.0); //->ArcadeDrive(0.0,0.0,true);
	}
}

//Send image to dashboard if IMAQ has not thrown an error
void LHSVision::SendToDashboard(Image *image, int error) // for both
{
	if(error < ERR_SUCCESS)
	{
		DriverStation::ReportError("Send To Dashboard error: " + std::to_string((long)imaqError) + "\n");
	}
	else
	{
		CameraServer::GetInstance()->SetImage(image); //originally binaryFrame
	}
}

//Converts a ratio with ideal value of 1 to a score. The resulting function is piecewise
//linear going from (0,0) to (1,100) to (2,0) and is 0 for all inputs outside the range 0-2
double LHSVision::RatioToScore(double ratio) // for both
{
	return (fmax(0, fmin(100*(1-fabs(1-ratio)), 100)));
}

double LHSVision::ConvexHullAreaScore(ParticleReportTote report) // for tote
{
	return RatioToScore((report.Area/report.ConvexHullArea)*1.18);
}

//Method to score if the particle appears to be a trapezoid. Compares the convex hull (filled in) area to the area of the bounding box.
//The expectation is that the convex hull area is about 95.4% of the bounding box area for an ideal tote.
double LHSVision::TrapezoidScore(ParticleReportTote report) // for tote
{
	return RatioToScore(report.ConvexHullArea/((report.BoundingRectRight-report.BoundingRectLeft)*(report.BoundingRectBottom-report.BoundingRectTop)*.954));
}

//Method to score if the aspect ratio of the particle appears to match the long side of a tote.
double LHSVision::LongSideScore(ParticleReportTote report) // for tote
{
	return RatioToScore(((report.BoundingRectRight-report.BoundingRectLeft)/(report.BoundingRectBottom-report.BoundingRectTop))/LONG_RATIO);
}

//Method to score if the aspect ratio of the particle appears to match the short side of a tote.
double LHSVision::ShortSideScore(ParticleReportTote report) // for tote
{
	return RatioToScore(((report.BoundingRectRight-report.BoundingRectLeft)/(report.BoundingRectBottom-report.BoundingRectTop))/SHORT_RATIO);
}

double LHSVision::ComputeDistanceTote(Image *image, ParticleReportTote report, bool isLong) // for tote
{
	double normalizedWidth, targetWidth;
	int xRes, yRes;

	imaqGetImageSize(image, &xRes, &yRes);
	normalizedWidth = 2*(report.BoundingRectRight - report.BoundingRectLeft)/xRes;
	SmartDashboard::PutNumber("Width", normalizedWidth);
	targetWidth = isLong ? 26.9 : 16.9;

	return  targetWidth/(normalizedWidth*12*tan(VIEW_ANGLE*M_PI/(180*2)));
}

//Computes the estimated distance to a target using the width of the particle in the image. For more information and graphics
//showing the math behind this approach see the Vision Processing section of the ScreenStepsLive documentation.
double LHSVision::ComputeDistanceTape(Image *image, ParticleReportTape report) // for tape
{
	double normalizedWidth, targetWidth;
	int xRes, yRes;

	imaqGetImageSize(image, &xRes, &yRes);
	normalizedWidth = 2*(report.BoundingRectRight - report.BoundingRectLeft)/xRes;
	SmartDashboard::PutNumber("Width", normalizedWidth);
	targetWidth = 7;

	return  targetWidth/(normalizedWidth*12*tan(VIEW_ANGLE*M_PI/(180*2)));
}

double LHSVision::AreaScore(ParticleReportTape report) // for tape
{
	double boundingArea = (report.BoundingRectBottom - report.BoundingRectTop) * (report.BoundingRectRight - report.BoundingRectLeft);
	//Tape is 7" edge so 49" bounding rect. With 2" wide tape it covers 24" of the rect.
	return RatioToScore((49.0/24.0)*report.Area/boundingArea);
}

//Method to score if the aspect ratio of the particle appears to match the retro-reflective target. Target is 7"x7" so aspect should be 1
double LHSVision::AspectScore(ParticleReportTape report) // for tape
{
	return RatioToScore(((report.BoundingRectRight-report.BoundingRectLeft)/(report.BoundingRectBottom-report.BoundingRectTop)));
}

bool LHSVision::TestVision() //by Michael
{
	if(mXbox->GetRawButton(XBOX_X_BUTTON) == true)
		return true;
	if(mXbox->GetRawButton(XBOX_A_BUTTON) == true)
	{
		while(mXbox->GetRawButton(XBOX_A_BUTTON) == true)
		{}
		if(send == 1)
		{
			StopCamera(1);
			StartCamera(2);
			DBSTR(1,"Camera 2");
			DBSTR(0,"Raw Image Mode");
			printf("Activating camera 2 - raw image only\n\n");
			send = 2;
		}
		else
		{
			StopCamera(2);
			StartCamera(1);
			DBSTR(1,"Camera 1");
			visionType = 3;
			printf("Activating camera 1 - processing available\n\n");
			send = 1;
		}
	}

	if(send == 1)
	{
		if(mXbox->GetRawButton(XBOX_RIGHT_BUMPER) == true)
		{
			visionType = 1; // tape mode
			printf("Tape Mode\n");
			DBSTR(0,"Tape Mode");
			DBLED(0,false);
		}
		if(mXbox->GetRawButton(XBOX_LEFT_BUMPER) == true)
		{
			visionType = 2;
			printf("Tote Mode\n");
			DBSTR(0,"Tote Mode");
			DBLED(1,false);
		}
		if(mXbox->GetRawButton(XBOX_Y_BUTTON) == true)
		{
			visionType = 3;
			printf("Raw Image Mode\n");
			DBSTR(0,"Raw Image Mode");
			DBLED(0,false);
			DBLED(1,false);
		}

		if(visionType == 1)
		{
			UseCameraForTape();
		}
		else if(visionType == 2)
		{
			UseCameraForTote();
		}
		else
		{
			TurnLightOff();
			IMAQdxGrab(session, frame, true, NULL);
			SendToDashboard(frame, imaqError);
			Wait(.1);
		}
	}
	else
	{
		TurnLightOff();
		IMAQdxGrab(session2, frame2, true, NULL);
		SendToDashboard(frame2, imaqError);
		Wait(.1);
	}
	return false;
}

#endif
