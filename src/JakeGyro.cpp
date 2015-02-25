/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "JakeGyro.h"
#include "I2C.h"
#include "HAL/HAL.hpp"
#include "LiveWindow/LiveWindow.h"
#include "Prefs.h"

// #include <stdio.h>

const uint8_t JakeGyro::kPowerCtlRegister;
const uint8_t JakeGyro::kDataFormatRegister;
const uint8_t JakeGyro::kDataRegister;
constexpr double JakeGyro::kGsPerLSB;

/**
 * Constructor.
 *
 * @param port The I2C port the gyro is attached to
 */
JakeGyro::JakeGyro(Port port):
		I2C(port, JAKE_GYRO_ADDRESS)
{
		uint8_t Buff[1];

		//m_i2c = new I2C((I2C::Port)port, kAddress);
		Read(0, 1, Buff);
		SmartDashboard::PutNumber("Jake Buff", Buff[0]);
		// printf("Jake Buff: %2.2X\n", Buff[0] & 0x00ff);

		// Turn on the measurements
		// Write(kPowerCtlRegister, kPowerCtl_Measure);
		// Specify the data format to read
		// SetRange(range);

		// HALReport(HALUsageReporting::kResourceType_ADXL345, HALUsageReporting::kJakeGyro, 0);
		// LiveWindow::GetInstance()->AddSensor("JakeGyro", port, this);
}

/**
 * Destructor.
 */
JakeGyro::~JakeGyro()
{
	//delete m_i2c;
	//m_i2c = NULL;
}

/** {@inheritdoc} */
void JakeGyro::SetRange(Range range)
{
	Write(kDataFormatRegister, kDataFormat_FullRes | (uint8_t)range);
}

/** {@inheritdoc} */
double JakeGyro::GetX()
{
	return GetAcceleration(kAxis_X);
}

/** {@inheritdoc} */
double JakeGyro::GetY()
{
	return GetAcceleration(kAxis_Y);
}

/** {@inheritdoc} */
double JakeGyro::GetZ()
{
	return GetAcceleration(kAxis_Z);
}

/**
 * Get the acceleration of one axis in Gs.
 *
 * @param axis The axis to read from.
 * @return Acceleration of the ADXL345 in Gs.
 */
double JakeGyro::GetAcceleration(JakeGyro::Axes axis)
{
	int16_t rawAccel = 0;
	//if(m_i2c)
	//{
		Read(kDataRegister + (uint8_t)axis, sizeof(rawAccel), (uint8_t *)&rawAccel);
	//}
	return rawAccel * kGsPerLSB;
}

/**
 * Get the acceleration of all axes in Gs.
 *
 * @return An object containing the acceleration measured on each axis of the ADXL345 in Gs.
 */
JakeGyro::AllAxes JakeGyro::GetAccelerations()
{
	AllAxes data = AllAxes();
	int16_t rawData[3];
	//if (m_i2c)
	//{
		Read(kDataRegister, sizeof(rawData), (uint8_t*)rawData);

		data.XAxis = rawData[0] * kGsPerLSB;
		data.YAxis = rawData[1] * kGsPerLSB;
		data.ZAxis = rawData[2] * kGsPerLSB;
	//}
	return data;
}

std::string JakeGyro::GetSmartDashboardType() {
	return "3AxisAccelerometer";
}

void JakeGyro::InitTable(ITable *subtable) {
	m_table = subtable;
	UpdateTable();
}

void JakeGyro::UpdateTable() {
	if (m_table != NULL) {
		m_table->PutNumber("X", GetX());
		m_table->PutNumber("Y", GetY());
		m_table->PutNumber("Z", GetZ());
	}
}

ITable* JakeGyro::GetTable() {
	return m_table;
}
