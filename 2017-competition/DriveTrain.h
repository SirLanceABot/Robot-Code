/*
 * DriveTrain.h
 *
 *  Created on: Jan 19, 2017
 *      Author: Programming
 */

#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <memory>
#include "PneumaticsDouble.h"
#include "ITG-3200.h"

class CANTalon;
namespace frc
{
class RobotDrive;
}

class DriveTrain
{
public:
	enum ShifterPosition {kLow, kHigh};

	static constexpr double WHEEL_BASE_CIRCUMFERENCE = 23.75 * 3.1415926;
	static constexpr double WHEEL_CIRCUMFERENCE = 6.0 * 3.1415926;
	static constexpr double TICKS_PER_INCH = (360.0 * 4.0 / 3.0) / WHEEL_CIRCUMFERENCE;
	static constexpr double ENCODER_CODES_PER_REV = (360.0 / 3.0) / WHEEL_CIRCUMFERENCE;

	DriveTrain(int leftMaster, int leftSlave1, int leftSlave2, int rightMaster, int rightSlave1, int rightSlave2, int lowSpeed, int highSpeed, frc::I2C::Port gyroPort, double sensitivity);
	void ArcadeDrive(double move, double rotate, bool square = true);
	void TankDrive(double left, double right);
	void ShiftSpeed();
	bool SpinCCW(double speed, double angle);
	bool NewSpinCW(double speed, double angle);
	bool NewSpinCCW(double speed, double angle);
	bool SpinCW(double speed, double angle);
	bool DriveDistance(double speed, double distance);
	double GetDistance();
	void ResetEncoders();
	void StopDriving();
	void ShiftDown();
	void ShiftUp();
	int GetLeftEncoder();
	int GetRightEncoder();
	void SetSafetyEnabled(bool safety);
	void MakeIsDoneDrivingTrue();
	bool NewDriveDistance(double speed, double distance);
	bool GetGearPos();
	bool IsGyroWorking();

	std::string GetShifterPos();


	void SetRampRate(int rate);
	void SetGyroAngle(double angleX, double angleY, double angleZ);
	double GetZ();
	void ResetGyro();

	void SetControlModeVoltage();
	void SetControlModePercentVBus();
	void Print();

private:
	std::shared_ptr<ITG3200> mGyro;
	std::shared_ptr<CANTalon> mLeftMaster;
	std::shared_ptr<CANTalon> mLeftSlave1;
//	std::shared_ptr<CANTalon> mLeftSlave2;
	std::shared_ptr<CANTalon> mRightMaster;
	std::shared_ptr<CANTalon> mRightSlave1;
//	std::shared_ptr<CANTalon> mRightSlave2;
	std::shared_ptr<PneumaticsDouble> mGearShift;
	std::shared_ptr<frc::RobotDrive> mDrive;

	bool mIsDoneDriving;

	ShifterPosition mPosition;

};



#endif /* DRIVETRAIN_H_ */
