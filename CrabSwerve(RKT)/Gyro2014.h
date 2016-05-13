/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef GYRO2014_H_
#define GYRO2014_H_

#include "SensorBase.h"
#include "PIDSource.h"
#include "LiveWindow/LiveWindowSendable.h"

class AnalogChannel;
class AnalogModule;

/**
 * Use a rate gyro to return the robots heading relative to a starting position.
 * The Gyro class tracks the robots heading based on the starting position. As the robot
 * rotates the new heading is computed by integrating the rate of rotation returned
 * by the sensor. When the class is instantiated, it does a short calibration routine
 * where it samples the gyro while at rest to determine the default offset. This is
 * subtracted from each sample to determine the heading. This gyro class must be used 
 * with a channel that is assigned one of the Analog accumulators from the FPGA. See
 * AnalogChannel for the current accumulator assignments.
 */
class Gyro2014 : public SensorBase, public PIDSource, public LiveWindowSendable
{
public:
	static const uint32_t kOversampleBits = 10;
	static const uint32_t kAverageBits = 0;
	static constexpr float kSamplesPerSecond = 50.0;
	static constexpr float kCalibrationSampleTime = 5.0;
	static constexpr float kDefaultVoltsPerDegreePerSecond = 0.007;

	Gyro2014(uint8_t moduleNumber, uint32_t channel);
	explicit Gyro2014(uint32_t channel);
	explicit Gyro2014(AnalogChannel *channel);
	explicit Gyro2014(AnalogChannel &channel);
	virtual ~Gyro2014();
	virtual float GetAngle();
	virtual double GetRate();
	void SetSensitivity(float voltsPerDegreePerSecond);
	void SetPIDSourceParameter(PIDSourceParameter pidSource);
	virtual void Reset();

	// PIDSource interface
	double PIDGet();
	
	void UpdateTable();
	void StartLiveWindowMode();
	void StopLiveWindowMode();
	std::string GetSmartDashboardType();
	void InitTable(ITable *subTable);
	ITable * GetTable();
	void Print();
	void GetValue();
	private:
	void InitGyro();

	AnalogChannel *m_analog;
	float m_voltsPerDegreePerSecond;
	double m_offset;
	bool m_channelAllocated;
	uint32_t m_center;
	PIDSourceParameter m_pidSource;
	
	ITable *m_table;
};

/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "AnalogChannel.h"
#include "AnalogModule.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"

const uint32_t Gyro2014::kOversampleBits;
const uint32_t Gyro2014::kAverageBits;
constexpr float Gyro2014::kSamplesPerSecond;
constexpr float Gyro2014::kCalibrationSampleTime;
constexpr float Gyro2014::kDefaultVoltsPerDegreePerSecond;

/**
 * Initialize the gyro.
 * Calibrate the gyro by running for a number of samples and computing the center value for this
 * part. Then use the center value as the Accumulator center value for subsequent measurements.
 * It's important to make sure that the robot is not moving while the centering calculations are
 * in progress, this is typically done when the robot is first turned on while it's sitting at
 * rest before the competition starts.
 */
void Gyro2014::InitGyro()
{
		m_table = NULL;
	if (!m_analog->IsAccumulatorChannel())
	{
		wpi_setWPIErrorWithContext(ParameterOutOfRange,
				"moduleNumber and/or channel (must be accumulator channel)");
		if (m_channelAllocated)
		{
			delete m_analog;
			m_analog = NULL;
		}
		return;
	}
	m_voltsPerDegreePerSecond = kDefaultVoltsPerDegreePerSecond;
	m_analog->SetAverageBits(kAverageBits);
	m_analog->SetOversampleBits(kOversampleBits);
	float sampleRate = kSamplesPerSecond *
		(1 << (kAverageBits + kOversampleBits));
	m_analog->GetModule()->SetSampleRate(sampleRate);
	Wait(1.0);

	m_analog->InitAccumulator();
	Wait(kCalibrationSampleTime);

	INT64 value;
	uint32_t count;
	m_analog->GetAccumulatorOutput(&value, &count);

	m_center = (uint32_t)((double)value / (double)count + .5); // tiny error in small sample for centering extrapolated to large drifts over time.
	// Drifts compounded somewhat by slight changes in temperature, non-level mounting of gyro, drift of power supply, noise

	m_offset = ((double)value / (double)count) - (double)m_center;

	m_analog->SetAccumulatorCenter(m_center);
	int32_t deadband = 550; // rkt
	m_analog->SetAccumulatorDeadband(deadband); ///< TODO: compute / parameterize this, rkt SET, was 0, fallacy in GetAngle uses count*offset even for samples that are within the deadband
	m_analog->ResetAccumulator();

	printf("\nInitGyro value %lld, count %lu, center %lu, deadband %ld, offset %f\n", value, count, m_center, deadband, m_offset);  // rkt
	printf("Voltage %f, %f\n", m_analog->GetAverageVoltage(), m_analog->GetVoltage());

	SetPIDSourceParameter(kAngle);

	nUsageReporting::report(nUsageReporting::kResourceType_Gyro, m_analog->GetChannel(), m_analog->GetModuleNumber() - 1);
	LiveWindow::GetInstance()->AddSensor("Gyro", m_analog->GetModuleNumber(), m_analog->GetChannel(), this);
}

/**
 * Gyro constructor given a slot and a channel.
 *
 * @param moduleNumber The analog module the gyro is connected to (1).
 * @param channel The analog channel the gyro is connected to (1 or 2).
 */
Gyro2014::Gyro2014(uint8_t moduleNumber, uint32_t channel)
{
	m_analog = new AnalogChannel(moduleNumber, channel);
	m_channelAllocated = true;
	InitGyro();
}

/**
 * Gyro constructor with only a channel.
 *
 * Use the default analog module slot.
 *
 * @param channel The analog channel the gyro is connected to.
 */
Gyro2014::Gyro2014(uint32_t channel)
{
	m_analog = new AnalogChannel(channel);
	m_channelAllocated = true;
	InitGyro();
}

/**
 * Gyro constructor with a precreated analog channel object.
 * Use this constructor when the analog channel needs to be shared. There
 * is no reference counting when an AnalogChannel is passed to the gyro.
 * @param channel The AnalogChannel object that the gyro is connected to.
 */
Gyro2014::Gyro2014(AnalogChannel *channel)
{
	m_analog = channel;
	m_channelAllocated = false;
	if (channel == NULL)
	{
		wpi_setWPIError(NullParameter);
	}
	else
	{
		InitGyro();
	}
}

Gyro2014::Gyro2014(AnalogChannel &channel)
{
	m_analog = &channel;
	m_channelAllocated = false;
	InitGyro();
}

/**
 * Reset the gyro.
 * Resets the gyro to a heading of zero. This can be used if there is significant
 * drift in the gyro and it needs to be recalibrated after it has been running.
 */
void Gyro2014::Reset()
{
	m_analog->ResetAccumulator();
}

/**
 * Delete (free) the accumulator and the analog components used for the gyro.
 */
Gyro2014::~Gyro2014()
{
	if (m_channelAllocated)
		delete m_analog;
}

/**
 * Return the actual angle in degrees that the robot is currently facing.
 *
 * The angle is based on the current accumulator value corrected by the oversampling rate, the
 * gyro type and the A/D calibration values.
 * The angle is continuous, that is can go beyond 360 degrees. This make algorithms that wouldn't
 * want to see a discontinuity in the gyro output as it sweeps past 0 on the second time around.
 *
 * @return the current heading of the robot in degrees. This heading is based on integration
 * of the returned rate from the gyro.
 */
float Gyro2014::GetAngle( void )
{
	INT64 rawValue;
	uint32_t count;
	m_analog->GetAccumulatorOutput(&rawValue, &count);

	INT64 value = rawValue - (INT64)((double)count * m_offset);

	double scaledValue = value * 1e-9 * (double)m_analog->GetLSBWeight() * (double)(1 << m_analog->GetAverageBits()) /
		(m_analog->GetModule()->GetSampleRate() * m_voltsPerDegreePerSecond);

	return (float)scaledValue;
}


/**
 * Return the rate of rotation of the gyro
 *
 * The rate is based on the most recent reading of the gyro analog value
 *
 * @return the current rate in degrees per second
 */
double Gyro2014::GetRate( void )
{
	return (m_analog->GetAverageValue() - ((double)m_center + m_offset)) * 1e-9 * m_analog->GetLSBWeight()
			/ ((1 << m_analog->GetOversampleBits()) * m_voltsPerDegreePerSecond);
}


/**
 * Set the gyro type based on the sensitivity.
 * This takes the number of volts/degree/second sensitivity of the gyro and uses it in subsequent
 * calculations to allow the code to work with multiple gyros.
 *
 * @param voltsPerDegreePerSecond The type of gyro specified as the voltage that represents one degree/second.
 */
void Gyro2014::SetSensitivity( float voltsPerDegreePerSecond )
{
	m_voltsPerDegreePerSecond = voltsPerDegreePerSecond;
}

void Gyro2014::SetPIDSourceParameter(PIDSourceParameter pidSource)
{
	if(pidSource == 0 || pidSource > 2)
		wpi_setWPIErrorWithContext(ParameterOutOfRange, "Gyro pidSource");
    m_pidSource = pidSource;
}

/**
 * Get the angle in degrees for the PIDSource base object.
 *
 * @return The angle in degrees.
 */
double Gyro2014::PIDGet()
{
	switch(m_pidSource){
	case kRate:
		return GetRate();
	case kAngle:
		return GetAngle();
	default:
		return 0;
	}
}

void Gyro2014::UpdateTable() {
	if (m_table != NULL) {
		m_table->PutNumber("Value", GetAngle());
	}
}

void Gyro2014::StartLiveWindowMode() {

}

void Gyro2014::StopLiveWindowMode() {

}

std::string Gyro2014::GetSmartDashboardType() {
	return "Gyro";
}

void Gyro2014::InitTable(ITable *subTable) {
	m_table = subTable;
	UpdateTable();
}

ITable * Gyro2014::GetTable() {
	return m_table;
}

void Gyro2014::GetValue(void)
	{
	INT64 value=0;
	uint32_t count=0;

	m_analog->GetAccumulatorOutput(&value, &count);
	printf("value %lld, count %lu, ", value, count);
	printf("Voltage %f, %f, ", m_analog->GetAverageVoltage(), m_analog->GetVoltage());
	}

void Gyro2014::Print() {

	printf("\n\nPrint the Gyro %p\n\n", this);

	INT64 value=0;
	uint32_t count=0;

	m_analog->GetAccumulatorOutput(&value, &count);

	printf("value %lld, count %lu, ", value, count);
	printf("value %lld, count %lu, ave value %ld, value %d\n",
		m_analog->GetAccumulatorValue(),
		m_analog->GetAccumulatorCount(),
		m_analog->GetAverageValue(),
		m_analog->GetValue());
	printf("Voltage %f, %f\n", m_analog->GetAverageVoltage(), m_analog->GetVoltage());
	printf("GetAverageBits %lu\n", m_analog->GetAverageBits());
	printf("GetLSBWeight %lu\n", m_analog->GetLSBWeight());
	printf("GetOffset %lu\n", m_analog->GetOffset());
	printf("GetOversampleBits %lu\n", m_analog->GetOversampleBits());
}

#endif
