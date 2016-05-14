#include "WPILib.h"
#include "ITG-3200.h"
#include "Constants.h"


class	Robot: public SampleRobot
{
public:
	Robot();
	~Robot();
	void OperatorControl();
private:
	Joystick* mXbox;
	RobotDrive* mDrive;
	ITG3200* mITG3200_1;
	CANTalon* frontLeft;
	CANTalon* backLeft;
	CANTalon* frontRight;
	CANTalon* backRight;
};

Robot::Robot()
{
	frontLeft = new CANTalon(3);
	backLeft = new CANTalon(0);
	frontRight = new CANTalon(1);
	backRight = new CANTalon(2);
	mXbox = new Joystick(0);
	mDrive = new RobotDrive(frontLeft, backLeft, frontRight, backRight);
	double angle[3] = {0.0, 0.0, 0.0};
	mITG3200_1 = new ITG3200(I2C::Port::kOnboard, GYRO::ITG3200_I2C_ADDR, GYRO::SAMPLE_RATE_5, angle);
	printf("Robot Constructor\n");
	mDrive->SetSafetyEnabled(false);
}

Robot::~Robot()
{
	delete frontLeft;
	delete backLeft;
	delete frontRight;
	delete backRight;
	delete mDrive;
	delete mXbox;
	delete mITG3200_1;
}

void Robot::OperatorControl()
{
	printf("OperatorControl\n");
	double angle[3] = {0.0, 0.0, 0.0};
	double angleRate[3] = {0.0, 0.0, 0.0};

	while (IsOperatorControl() && IsEnabled())
	{
		double pitch, roll, yaw;
		yaw = mITG3200_1->GetZ();
		pitch = mITG3200_1->GetY();
		roll = mITG3200_1->GetX();
		printf("Yaw: %f		Pitch: %f	Roll: %f\n", yaw, pitch, roll);
		float a = mXbox->GetRawAxis(4);
		float b = mXbox->GetRawAxis(0);
		float c = mXbox->GetRawAxis(5);
//		double z = mITG3200_1->GetZ();
//		int e = ((int)z) % 360;
//
		if (fabs(a) < .15)
			a = 0;
		if (fabs(b) < .15)
			b = 0;
		if (fabs(c) < .15)
			c = 0;
//
//
//				int num_of_rots = (int)z / 360; 	// this gets the number of rotations the robot did. Positive and negative
//				//		 rotate the robot if its going off course
//				// rotate the robot back onto course if it has rotate less than
//				if (num_of_rots == 0 && e < -8)
//					mDrive->MecanumDrive_Cartesian(a, -0.4 + b, c);
//				else if (num_of_rots == 0 && e > 8)
//					mDrive->MecanumDrive_Cartesian(a, 0.4 + b, c);
//
//				// rotate the robot back on course if it has rotated an entire complete clockwise rotation
//				else if (num_of_rots < 0 && (e > -8 && e < -354))
//					mDrive->MecanumDrive_Cartesian(a, b, c);
//				else if (num_of_rots < 0 && (e < -9 && e > -180))
//					mDrive->MecanumDrive_Cartesian(a, -0.4 + b, c);
//				else if (num_of_rots < 0 && (e > -355 && e < -181))
//					mDrive->MecanumDrive_Cartesian(a, 0.4 + b, c);
//
//				// rotate the robot back on course if it has rotated an entire complete counter clockwise rotation
//				else if (num_of_rots > 1 && (e < 8 && e > 354))
//					mDrive->MecanumDrive_Cartesian(a, b, c);
//				else if (num_of_rots > 1 && (e > 9 && e < 180))
//					mDrive->MecanumDrive_Cartesian(a, 0.4 + b, c);
//				else if (num_of_rots > 1 && (e < 355 && e > 181))
//					mDrive->MecanumDrive_Cartesian(a, -0.4 + b, c);
//
//				// normal mechanum drive, strafing, doesn't work. I don't know why though
//				else
					mDrive->MecanumDrive_Cartesian(a, b, c);
//
//				printf("Value: %d		Mod 360: %d		Rotations: %d\n", (int)z, e, num_of_rots);


				Wait(.02);
	}
}

START_ROBOT_CLASS(Robot)
