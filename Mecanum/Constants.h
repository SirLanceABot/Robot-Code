/*
 * Constants.h
 *
 *  Created on: Feb 18, 2016
 *      Author: Programming
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

namespace GYRO
{
// id
const int ITG3200_ID_REGISTER = 0x00;
const int ITG3200_ID = 0x34;
const int ITG3200_ID_BITS = 0x7e;

// configuration registers
const int ITG3200_SMPLRT_DIV = 0x15;
const int ITG3200_DLPF_FS = 0x16;

// interrupt registers
const int ITG3200_INT_CFG = 0x17;
const int ITG3200_INT_STATUS = 0x1A;

// data registers (read only)
const int ITG3200_TEMP_H = 0x1B;
const int ITG3200_TEMP_L = 0x1C;
const int ITG3200_XOUT_H = 0x1D;
const int ITG3200_XOUT_L = 0x1E;
const int ITG3200_YOUT_H = 0x1F;
const int ITG3200_YOUT_L = 0x20;
const int ITG3200_ZOUT_H = 0x21;
const int ITG3200_ZOUT_L = 0x22;
const int DATA_REG_SIZE = 8;

// power management
const int ITG3200_PWR_MGM = 0x3E;

// useful values
const int ITG3200_RESET = 0x80;
const int ITG3200_SLEEP = 0x40;
const int ITG3200_WAKEUP = 0x00;
// FS=11 DLPF=000 => 11000 => 0x18 => 0b11'000
const int ITG3200_FS_3 = 0x18;

const int ITG3200_I2C_ADDR = 0x68;
const int ITG3200_I2C_ADDR_ALT = 0x69;

// sample rate constants
const int SAMPLE_RATE_0 = 0; // 256Hz low pass filter bandwidth		8,000Hz Internal Sample Rate
const int SAMPLE_RATE_1 = 1; // 188Hz low pass filter bandwidth		1,000Hz Internal Sample Rate
const int SAMPLE_RATE_2 = 2; // 98Hz low pass filter bandwidth		1,000Hz Internal Sample Rate
const int SAMPLE_RATE_3 = 3; // 42Hz low pass filter bandwidth		1,000Hz Internal Sample Rate
const int SAMPLE_RATE_4 = 4; // 20Hz low pass filter bandwidth		1,000Hz Internal Sample Rate
const int SAMPLE_RATE_5 = 5; // 10Hz low pass filter bandwidth		1,000Hz Internal Sample Rate
const int SAMPLE_RATE_6 = 6; // 5Hz low pass filter bandwidth		1,000Hz Internal Sample Rate
}



#endif /* CONSTANTS_H_ */
