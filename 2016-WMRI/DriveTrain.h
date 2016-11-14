/*
 * TankDrive.h
 *
 *  Created on: Jan 23, 2016
 *      Author: 19mbommineni
 */

#ifndef DRIVETRAIN_H_
#define DRIVETRAIN_H_

#include <memory>


class CANTalon;
class RobotDrive;
class XboxJoystick;

class DriveTrain
{
public:
	DriveTrain(int, int, int, int);
	~DriveTrain();
	void TankDrive(float, float);
	void ArcadeDrive(float, float);
	void ModifiedTankDrive(float, float);
	void Stop();
	bool DriveDistance(int, float);
	bool Spin180();
	void Test(std::shared_ptr<XboxJoystick>);
	void ResetDriveEncoder();
//	enum encoderPos {kLeftDriveEncoder, kRightDriveEncoder};
	int GetLeftDriveEncoder();
	int GetRightDriveEncoder();

private:
	std::shared_ptr<CANTalon> mFrontLeftMotor;
	std::shared_ptr<CANTalon> mRearLeftMotor;
	std::shared_ptr<CANTalon> mFrontRightMotor;
	std::shared_ptr<CANTalon> mRearRightMotor;
	std::shared_ptr<RobotDrive> mRobotDrive;
/*
	CANTalon* mFrontLeftMotor;
	CANTalon* mRearLeftMotor;
	CANTalon* mFrontRightMotor;
	CANTalon* mRearRightMotor;
	RobotDrive* mRobotDrive;
*/
	bool mDoneDriving;
};



#endif /* DRIVE_TRAIN_SRC_DRIVETRAIN_H_ */
