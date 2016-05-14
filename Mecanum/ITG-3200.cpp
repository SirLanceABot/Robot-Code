/*
 * ITG-3200.cpp
 *
 *  Created on: Feb 16, 2016
 *      Author: Robotics
 */

#include "ITG-3200.h"
#include "WPILib.h"
#include "Constants.h"

ITG3200::ITG3200(I2C::Port port, uint8_t deviceAddress, int sampleRate, double angle[], float sensitivity_Scale_Factor)//:
//		mGyro(std::make_unique<I2C>(port, deviceAddress)),
//		mTimer(std::make_unique<Timer>()),
//		mSensitivity(Sensitivity_Scale_Factor),
//      mAngle{0.0, 0.0, 0.0}
{
	mGyro = new I2C(port, deviceAddress);
	mTimer = new Timer();
	mSensitivity = sensitivity_Scale_Factor;
	mAngle[0] = 0.0;
	mAngle[1] = 0.0;
	mAngle[2] = 0.0;
	printf("[ITG-3200] %p Compiled file:%s, date:%s, time:%s, function:%s, function:%s, printed at line:%d\n",
			this, __FILE__, __DATE__, __TIME__, __FUNCTION__, __PRETTY_FUNCTION__, __LINE__);

	mTimer->Start();

	//			printf("Status=:%#.2x\n", mBuffer[0]);

	Wait(.1);

	mGyro->Write(GYRO::ITG3200_PWR_MGM, GYRO::ITG3200_RESET);

	Wait(.1);

	mGyro->Read(GYRO::ITG3200_ID_REGISTER, 1, mBuffer);

	mWorking = (int8_t)(mBuffer[0] & GYRO::ITG3200_ID_BITS) >> 1 == GYRO::ITG3200_ID;

	if (IsWorking())
	{
		printf("[ITG-3200] gyro working on port %x, address %x, device ID register %x\n",
				port, deviceAddress, mBuffer[0]);

		SetSampleRate(sampleRate); // default 256 Hz low pass filter + 8kHz sample rate

		Wait(0.7);

		printf("[ITG-3200] starting calibration - do not vibrate gyro until completed\n");

		Calibrate();

		SetAngle(angle);

		mPrevTime = mTimer->Get();

		Get();
	}
	else
	{
		printf("[ITG-3200] gyro NOT working on port %x, address %x, device ID register %x\n", port, deviceAddress, mBuffer[0]);
	}
}

void ITG3200::Calibrate()
{
	int reads = 600;
	double delay = 0.004; // 4 milliseconds
	int skip = 5; // initial samples to skip
	double temp[3] = {0.0, 0.0, 0.0};

	for(int i = 0; i < reads; i++)
	{
		GetRaw();
		if (i >= skip)
		{
			for (int j = 0; j < 3; j++)
			{
				temp[j] += mRotation[j];
			}
		}
		Wait(delay);
	}

	printf("[ITG-3200] completed calibrated offset of X Y Z ");
	for(int i = 0; i < 3; i++)
	{
		mOffset[i] = -(float)temp[i] / (float)(reads - skip);
		printf("%f", mOffset[i]);
	}
	printf("\n");

}

ITG3200::~ITG3200()
{
	delete mGyro;
	delete mTimer;
}

void ITG3200::GetAngle(double angle[])
{
	angle[0] = mAngle[0];
	angle[1] = mAngle[1];
	angle[2] = mAngle[2];
}


void ITG3200::GetAngleRate(double angleRate[])
{
	angleRate[0] = mAngleRate[0];
	angleRate[1] = mAngleRate[1];
	angleRate[2] = mAngleRate[2];
}

float ITG3200::GetTemperature()
{
	return 35.0 + (mTemperature + 13200.0) / 280.0;
}

void ITG3200::GetRaw()
{
	mGyro->Read(GYRO::ITG3200_TEMP_H, 8, mBuffer);
	mTemperature = (float)(int16_t)(((mBuffer[0] << 8 ) | mBuffer[1]));  // temperature
	mRotation[0] = (float)(int16_t)(((mBuffer[2] << 8 ) | mBuffer[3]));	// x
	mRotation[1] = (float)(int16_t)(((mBuffer[4] << 8 ) | mBuffer[5]));	// y
	mRotation[2] = (float)(int16_t)(((mBuffer[6] << 8 ) | mBuffer[7]));	// z

	//			printf("Temperature %#.2x %#.2x %f\n",
	//					mBuffer[0], mBuffer[1], mTemperature);
	//			printf("X %#.2x %#.2x %f Y %#.2x %#.2x %f Z %#.2x %#.2x %f\n",
	//					mBuffer[2], mBuffer[3], mRotation[0], mBuffer[4], mBuffer[5], mRotation[1], mBuffer[6], mBuffer[7], mRotation[2]);
}

void ITG3200::Get()
{
	double current_time = mTimer->Get();

	if (current_time < mPrevTime)
		current_time = 0.005; // some clocks rollover often.  Use an arbitrary small increment for the last time step.

	GetRaw();

	for(int i = 0; i < 3; i++)
	{
		mAngleRate[i] = (mRotation[i] + mOffset[i])/mSensitivity; // convert raw sensor to angular rate
		mAngle[i] += mAngleRate[i] * (current_time - mPrevTime); // integrate angular rate to yield angle
	}

	mPrevTime = current_time;
}

bool ITG3200::IsWorking()
{
	return mWorking;
}

void ITG3200::SetAngle(double angle[])
{
	mAngle[0] = angle[0];
	mAngle[1] = angle[1];
	mAngle[2] = angle[2];
	mPrevTime = mTimer->Get();
}

void ITG3200::SetSampleRate(uint8_t sampleRate) // sampleRate is the digital Low Pass Filter Configuration
{
	uint8_t dLPFFS;	//This is the digital low pass filterfulll scale

	if (sampleRate < 0 || sampleRate > 7)
		sampleRate = 0;

	dLPFFS = GYRO::ITG3200_FS_3 | sampleRate;

	mGyro->Write(GYRO::ITG3200_DLPF_FS, dLPFFS);

	printf("[ITG-3200] DLPF, Full Scale, %#.2x\n", dLPFFS);

	Wait(0.06);
}

void ITG3200::UpdateGyro(double angle[], double angleRate[])
{
	Get();
	GetAngle(angle);
	GetAngleRate(angleRate);
}

void ITG3200::TestGyro()
{
	double angle[3];
	double angleRate[3];
	UpdateGyro(angle, angleRate);
	printf("[ITG3200_1] temperature: %7.2f X: %9.2f Y: %9.2f Z: %9.2f Xrate: %9.2f Yrate: %9.2f Zrate: %9.2f\n",
			GetTemperature(),
			angle[0], angle[1], angle[2],
			angleRate[0], angleRate[1], angleRate[2]);
}

double ITG3200::GetZ()
{
	double angle[3];
	double angleRate[3];
	UpdateGyro(angle, angleRate);
	return angle[2];
}

double ITG3200::GetY()
{
	double angle[3];
	double angleRate[3];
	UpdateGyro(angle, angleRate);
	return angle[1];
}

double ITG3200::GetX()
{
	double angle[3];
	double angleRate[3];
	UpdateGyro(angle, angleRate);
	return angle[0];
}
