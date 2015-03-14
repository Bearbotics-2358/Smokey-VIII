#include <Prefs.h>
#include "JakeGyro.h"
#include "I2C.h"
#include "HAL/HAL.hpp"
#include "LiveWindow/LiveWindow.h"

const uint8_t JakeGyro::kPowerMgmRegister;
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
		lastUpdate = 0;
		angle = 0;
		Init();
		//m_i2c = new I2C((I2C::Port)port, kAddress);
		// int ret = Read(0, 1, Buff);
		// printf("Jake Buff: %2.2X\n", Buff[0] & 0x00ff);

		// Turn on the measurements
		// Write(kPowerCtlRegister, kPowerCtl_Measure);
		// Specify the data format to read
		//SetRange(range);

		// HALReport(HALUsageReporting::kResourceType_ADXL345, HALUsageReporting::kJakeGyro, 0);
		// LiveWindow::GetInstance()->AddSensor("JakeGyro", port, this);
}

JakeGyro::~JakeGyro()
{
	//delete m_i2c;
	//m_i2c = NULL;
}

void JakeGyro::Init()
{
	lastUpdate = 0;
	Write(kDLPFRegister, 0x1B);
	Write(kSampleRateDivider, 9);
	Write(kPowerMgmRegister, 1);
	Write(kIntCfg, 1);
	uint8_t stat;

	angleBias = 0;
	for(int i = 0; i < 10; i++) {
		do {
			Read(kIntStatus, 1, &stat);
		} while(!(stat & 1));
		Update();
		angleBias += XAxis;
	}
	angleBias /= 10;
	// SmartDashboard::PutNumber("Angle Bias", angleBias);
	angle = 0;
}

uint8_t JakeGyro::GetReg0()
{
	uint8_t id;
	Read(0, 1, &id);
	SmartDashboard::PutNumber("Gyro ID", id);

	return id;
}

int16_t JakeGyro::GetReg(uint8_t regNum)
{
	uint16_t ret;
	uint8_t buff[2];

	Read(regNum, 2, buff);
	ret = (buff[0] << 8) | buff[1];
	return (int16_t)ret;
}

void JakeGyro::Update()
{
	if(lastUpdate == 0){
		lastUpdate = Timer::GetFPGATimestamp();
		return;
	}
	double time = Timer::GetFPGATimestamp();
	double timeDelta = (time - lastUpdate);

	temperature = GetReg(kTempRegister);
	temperature = -13200 - temp;
	temperature = temperature / 280;
	temperature += 35;

	XAxis = GetReg(kDataRegister + kAxis_X);
	XAxis = XAxis / 14.375;

	YAxis = GetReg(kDataRegister + kAxis_Y);
	YAxis = YAxis / 14.375;

	ZAxis = GetReg(kDataRegister + kAxis_Z);
	ZAxis = ZAxis / 14.375;

	angle += (XAxis - angleBias) * timeDelta;
	lastUpdate = time;

	if (angle > 360) {
		angle -= 360;
	}

	else if (angle < 0) {
		angle += 360;
	}

}

double JakeGyro::GetX()
{
	return XAxis;
}

double JakeGyro::GetY()
{
	return YAxis;
}

double JakeGyro::GetZ()
{
	return ZAxis;
}

std::string JakeGyro::GetSmartDashboardType() {
	return "3AxisAccelerometer";
}

int JakeGyro::GetTemp() {
	return temperature;
}

double JakeGyro::GetAngle()
{
	return angle;
}

void JakeGyro::Reset() {
	angle = 0;
}
