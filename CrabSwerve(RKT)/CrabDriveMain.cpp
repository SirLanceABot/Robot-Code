/*
Pseudo Swerve Crab drive
Right 2 wheels steer together
Back 2 wheels steer together
Left 2 wheels drive together
Right 2 wheels drive together

Gyro shows field orientation of the front of the chassis
Front steering wheel and back steering wheel rotary encoders show wheels' orientations with respect to the chassis

Left and right drive chains must start to the right of their wheels
*/
class	CrabDriveSteer;
class	DriveTrain;
#include "WPILib.h"
class CrabDriveMain : public SimpleRobot
{
public:
	CrabDriveMain(void);
	~CrabDriveMain(void);
	void RobotInit(void);
	void Disabled(void);
	void Autonomous(void);
	void OperatorControl(void);
	void Test(void);
	void ReverseTest(void);
	void TestRKT1(void);
	void TankDriveOld(void);
	void DriveTest(void);
	void SetWheelPosition(void);
	void SideCarTest(int);
	void DriveTrainTest(void);
	void TightTurn(double);
	void SpinRobot(double);
	void TankDrive(void);

private: 
	CrabDriveSteer* mCrabDriveSteer;
	DriveTrain* mDriveTrain;
	//DriverStation* mDriverStation;

	double mSpeed;
};
#include "Headers.h"
CrabDriveMain::CrabDriveMain(void)
{
	printf("File %s Date %s Time %s\n",__FILE__,__DATE__, __TIME__);
	mCrabDriveSteer = new CrabDriveSteer();
	mDriveTrain = new DriveTrain();
	mXBoxJoystick = new XBoxJoystick();
	mSpeed = 0.0;
	timer->Reset();
	timer->Start();
}

CrabDriveMain::~CrabDriveMain(void)
{
	delete mCrabDriveSteer;
	mCrabDriveSteer = NULL;
	delete mDriveTrain;
	mDriveTrain = NULL;
	delete mXBoxJoystick;
	mXBoxJoystick = NULL;
}

void CrabDriveMain::RobotInit(void)
{
	printf("\n\n*** Robot Init Function ***\n\n");
}

void CrabDriveMain::Disabled(void)
{
	printf("\n\n*** Robot Disabled Function ***\n\n");
	mCrabDriveSteer->StopAllWheels();
	mDriveTrain->Stop();
}

void CrabDriveMain::Autonomous(void)
{
	printf("Autonomous Mode\n");
}

void CrabDriveMain::OperatorControl(void)
{
	while(IsOperatorControl())
	{
		mXBoxJoystick->Print(this);
		//DriveTest();
		Wait(.1021);
	}

}

void CrabDriveMain::ReverseTest(void)
{   // RK Thomas 10/2015
	// test how fast motor can change directions before D-Link crashes
	// cRIO with Talon SR and controller braking on
	// CIM at power=1.0 has to wait disabled for .13 seconds before switching from +1 to -1
	// CIM at power= .7 has to wait disabled for .0076 seconds before switching from +.7 to -.7
	// CIM at power= .5 has to wait disabled for .0026 seconds before switching from +.5 to -.5
	// CIM at power= .3 power can switch +.3 to -.3 instantaneously - no disabled waiting time needed

	bool runTest;
	double leftDriveSpeed, rightDriveSpeed, DriveSpeed;
	double WaitTime;

	DriveSpeed = .5;

	while(IsTest())
	{
		WaitTime = .1;
		runTest = true;

		while(runTest)
		{
			printf("\n%f, Will Wait %f between direction change\n", timer->Get(), WaitTime);
			fflush(stdout);
			Wait(.5);

			leftDriveSpeed = rightDriveSpeed = DriveSpeed;
			mDriveTrain->Drive(leftDriveSpeed, rightDriveSpeed);  // drive motors left, right
			Wait(1.);
			mDriveTrain->Stop();
			Wait(WaitTime);
			leftDriveSpeed = rightDriveSpeed = -DriveSpeed;
			mDriveTrain->Drive(leftDriveSpeed, rightDriveSpeed);  // drive motors left, right
			Wait(1.);
			mDriveTrain->Stop();
			Wait(1.);

			WaitTime = WaitTime*.9;
		}
	}
}

void CrabDriveMain::TestRKT1(void)
{
	printf("Sensors JLX: JLY: ELDist: ERDist: ELRate: ELRaw: ERRaw: ERRate: LSpeed: RSpeed: EF: EB: CF: CB: GA:\n");

	while(IsTest())
	{
		printf("\n%f ", timer->Get());
		mXBoxJoystick->Print(this);
		mDriveTrain->Print();
		mCrabDriveSteer->Print();
		mCrabDriveSteer->SetFrontWheels(0); // front steer motor
		mCrabDriveSteer->SetBackWheels(0); // back steer motor
		mDriveTrain->Drive(.0, 0.5);  // drive motors (left, right)
		Wait(.02);
	}
}

void CrabDriveMain::Test(void)
{
	double RobotHeading;

	while(IsTest())
	{
		printf("%f, ", timer->Get());

		RobotHeading = mCrabDriveSteer->GetRobotHeading();

		if (RobotHeading > 350.) RobotHeading -= 360.;

		mCrabDriveSteer->GyroGetValue();
		printf("%f, %ld, %ld\n", RobotHeading, mCrabDriveSteer->GetGyroTemperature(), mCrabDriveSteer->GetAnalogVoltage());

		Wait(.25);
	}


//
//		printf("%f, %ld, %ld", RobotHeading, mCrabDriveSteer->GetGyroTemperature(), mCrabDriveSteer->GetAnalogVoltage());
//	double RobotHeading;
//
//	while(IsTest())
//	{
//		printf("%f, ", timer->Get());
//		RobotHeading = mCrabDriveSteer->GetRobotHeading();
//		if (RobotHeading > 350.) RobotHeading -= 360.;
//		printf("%f, %ld, %ld", RobotHeading, mCrabDriveSteer->GetGyroTemperature(), mCrabDriveSteer->GetAnalogVoltage());
//		if (RobotHeading > 9.)
//		{
//			mCrabDriveSteer->CalibrateWheels(7);
//			printf(" , "" reset gyro""");
//		}
//		printf("\n");
//		Wait(.25);
////		printf("%f Set Wheel Position with buttons Front A=+Right, B=-Left, Back X=+Right, Y=-Left, Reset Gyro=BACK, Reset Encoder=START\n", timer->Get());
////		SetWheelPosition();
////		printf("Robot Heading = %f ", mCrabDriveSteer->GetRobotHeading());
////		Wait(.01);
//	}
}

void CrabDriveMain::DriveTest() //  RKT good function
{
	double x, y, xRight, driveAngle = 0.000, driveSpeed; //, spinAngle;
	int leftDirection, rightDirection;
	bool leftBumper, rightBumper;

	//mDriveTrain->SetSafetyEnabled(false);

	x = mXBoxJoystick->GetAxis(XBoxJoystick::kLeftXAxis);
	y = mXBoxJoystick->GetAxis(XBoxJoystick::kLeftYAxis);
	xRight = mXBoxJoystick->GetAxis(XBoxJoystick::kRightXAxis);

	leftBumper   = mXBoxJoystick->GetXBoxButton(XBoxJoystick::kLBumper);
	rightBumper  = mXBoxJoystick->GetXBoxButton(XBoxJoystick::kRBumper);

	if((fabs(x) > .2 || fabs(y) > .2) && !leftBumper && !rightBumper) // left stick moved and no bumpers - crab drive
	{
		printf("\n%f ", timer->Get());
		driveAngle = mXBoxJoystick->GetLeftJoystickAngle();
		driveSpeed = mXBoxJoystick->GetLeftJoystickHypotenuse();
		printf("Angle: %f Speed: %f Gryo: %f\n", driveAngle, driveSpeed, mCrabDriveSteer->GetRobotHeading());
		mCrabDriveSteer->UpdateSteer(driveAngle);
		rightDirection = mCrabDriveSteer->GetRightDirection();
		leftDirection = mCrabDriveSteer->GetLeftDirection();
		printf("DS: %f  ", driveSpeed);
		printf("Left: %d Right: %d\n", leftDirection, rightDirection);
		mDriveTrain->UpdateDrive(driveSpeed * leftDirection, driveSpeed * rightDirection);
	}
	else if(fabs(xRight) > .2 && !leftBumper && rightBumper) // right stick x moved and right bumper and no left bumper - spin
	{
		SpinRobot(xRight);
	}
	else if((fabs(x) > .2 || fabs(y) > .2) && leftBumper && !rightBumper) // left stick moved and left bumper and no right bumper- tank drive
	{
		TankDrive();
	}
	else // any other combo - stop
	{
		mDriveTrain->Stop();
		mCrabDriveSteer->StopAllWheels();
	}
}

void CrabDriveMain::SetWheelPosition(void)
{
	mCrabDriveSteer->CalibrateWheels();
}

void CrabDriveMain::SideCarTest(int port)
{
}

void CrabDriveMain::DriveTrainTest(void)
{
	double speed;
	speed = mXBoxJoystick->GetAxis(XBoxJoystick::kLeftYAxis);
	if(fabs(speed) > .1)
	{
		mDriveTrain->UpdateDrive(speed, speed);
	}
	else
	{
		mDriveTrain->UpdateDrive(0, 0);
	}
}

void CrabDriveMain::TankDriveOld(void)
{
	//  needs work.  The angles are no longer computed correctly.  I don't like the assumptions and limitations.

	double leftY, rightY, robotHeading, frontHeading, backHeading;
	leftY = mXBoxJoystick->GetAxis(XBoxJoystick::kLeftYAxis);
	rightY = mXBoxJoystick->GetAxis(XBoxJoystick::kRightYAxis);
	robotHeading = mCrabDriveSteer->GetRobotHeading();
	frontHeading = mCrabDriveSteer->GetFrontHeading();
	backHeading = mCrabDriveSteer->GetBackHeading();

	if(fabs(rightY) > .1 && fabs(leftY) > .1) // if both sticks active positive then drive.  Why must both be pushed and positive?
	{// see if chassis and wheels aligned - pointing either the same direction OR 180 apart - both ways OK
		if((fabs((robotHeading + 90) - frontHeading) < MIN_ANGLE_TOLERANCE && fabs((robotHeading + 90) - backHeading) < MIN_ANGLE_TOLERANCE) ||
				(fabs((robotHeading + 270) - frontHeading) < MIN_ANGLE_TOLERANCE && fabs((robotHeading + 270) - backHeading) < MIN_ANGLE_TOLERANCE))
		{
			mDriveTrain->UpdateDrive(leftY, rightY); // wheels and chassis aligned so drive
		}
		else // wheels and chassis not aligned so stop driving and align wheels to the chassis.  Why stop driving?
		{
			mDriveTrain->Stop();
			mCrabDriveSteer->UpdateSteer(robotHeading + 90);
		}
	}
}

void CrabDriveMain::TankDrive(void)
{   // Tank Turn
	// Uses the 2 sticks of the XBox.
	// Driving is relative to the robot (as if the driver is in the robot and is facing forward)
	// That is the steering encoders aren't used in any way and shouldn't be changing and front/back left/right are relative to the encoders being
	// firmly attached to the chassis.
	// But there could be a little drift of the 2 steering mechanisms so actively align the wheels with the chassis if the robot is driving.

	double leftDriveSpeed, rightDriveSpeed;

		printf("\n%f ", timer->Get());

		leftDriveSpeed = -mXBoxJoystick->GetAxis(XBoxJoystick::kLeftYAxis);
		rightDriveSpeed = -mXBoxJoystick->GetAxis(XBoxJoystick::kRightYAxis);

		if (fabs(leftDriveSpeed) < .2) leftDriveSpeed = 0.;   // need minimum stick movement to cause driving
		if (fabs(rightDriveSpeed) < .2) rightDriveSpeed = 0.;

		if (fabs(leftDriveSpeed) != 0. || fabs(rightDriveSpeed) != 0.) // check for driving requested
			{
			mCrabDriveSteer->UpdateSteer(mCrabDriveSteer->GetRobotHeading()); // keep wheels aligned with chassis

			if (leftDriveSpeed >  1.) leftDriveSpeed =  1.;
			if (leftDriveSpeed < -1.) leftDriveSpeed = -1.;
			leftDriveSpeed = leftDriveSpeed*leftDriveSpeed*leftDriveSpeed; // stick less responsive down low

			if (rightDriveSpeed >  1.) rightDriveSpeed =  1.;
			if (rightDriveSpeed < -1.) rightDriveSpeed = -1.;
			rightDriveSpeed = rightDriveSpeed*rightDriveSpeed*rightDriveSpeed; // stick less responsive down low
			}

		mDriveTrain->Drive(leftDriveSpeed, rightDriveSpeed);  // drive motors left, right
}

void CrabDriveMain::SpinRobot(double direction)
{
	double frontHeading = mCrabDriveSteer->GetFrontHeading();
	double backHeading = mCrabDriveSteer->GetBackHeading();
	double robotHeading = mCrabDriveSteer->GetRobotHeading();

	// Wheels must be sort of close to pointing forward/backward with respect to the chassis in order to do a spin "tank" turn
	// Align the wheels if need be

	if((fabs(robotHeading - frontHeading) < MID_ANGLE_TOLERANCE
			&& fabs(robotHeading - backHeading) < MID_ANGLE_TOLERANCE)
		||
		(fabs(mCrabDriveSteer->NormalizeAngle(robotHeading + 180) - frontHeading) < MID_ANGLE_TOLERANCE
			&& fabs(mCrabDriveSteer->NormalizeAngle(robotHeading + 180) - backHeading) < MID_ANGLE_TOLERANCE))
	{ // Wheels straight enough to spin so do so
		if(direction > 0)
		{
			mDriveTrain->SpinRobot( mCrabDriveSteer->GetLeftDirection());
		}
		if(direction < 0)
		{
			mDriveTrain->SpinRobot(-mCrabDriveSteer->GetLeftDirection());
		}
	}
	else // Wheels not aligned well enough for a tank turn.  Straighten them out for the spin
	{
		mDriveTrain->Stop();  // no need to be driving as before while aligning wheels for the spin
		mCrabDriveSteer->UpdateSteer(robotHeading); // align wheels with the chassis
	}
}

void CrabDriveMain::TightTurn(double direction)
{
	int leftDirection, rightDirection;

	if(mCrabDriveSteer->TightTurn(-45))
	{
		leftDirection = mCrabDriveSteer->GetLeftDirection();
		rightDirection = mCrabDriveSteer->GetRightDirection();

		if(direction > 0)
			mDriveTrain->UpdateDrive(.5 * leftDirection, .5 * rightDirection);
		if(direction < 0)
			mDriveTrain->UpdateDrive(-.5 * leftDirection, -.5 * rightDirection);
	}
	else
	{
		mDriveTrain->Stop();
	}
}

START_ROBOT_CLASS(CrabDriveMain);
