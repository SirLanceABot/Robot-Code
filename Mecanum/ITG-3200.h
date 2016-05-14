/*
 * ITG-3200.h
 *
 *  Created on: Feb 16, 2016
 *      Author: Robotics
 */

#ifndef SRC_ITG_3200_H_
#define SRC_ITG_3200_H_

#include "wpilib.h"
#include "Constants.h"

class ITG3200
{
public:
	ITG3200(I2C::Port port, uint8_t deviceAddress, int sampleRate, double angle[], float sensitivity_Scale_Factor = 14.375);
	~ITG3200();
	bool IsWorking();
	void Calibrate();
	void Get();
	void GetAngle(double angle[]);
	void GetAngleRate(double angleRate[]);
	float GetTemperature();
	void SetAngle(double angle[]);
	void SetSampleRate(uint8_t sampleRate);
	void TestGyro();
	void UpdateGyro(double angle[], double angleRate[]);
	double GetZ(); // returns the yaw (rotation)
	double GetX(); // returns the roll
	double GetY(); // returns the pitch

private:
//	std::unique_ptr<I2C> mGyro;
	I2C* mGyro;
//	std::unique_ptr<Timer> mTimer;
	Timer* mTimer;
	void GetRaw();
	float mSensitivity;
	double mAngle[3];
	uint8_t mBuffer[8];
	float mOffset[3];
	float mAngleRate[3];
	float mTemperature;
	float mRotation[3];
	double mPrevTime;
	bool mWorking;
};

#endif /* SRC_ITG_3200_H_ */
