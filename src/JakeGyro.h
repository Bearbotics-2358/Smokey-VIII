#pragma once

#include "I2C.h"
#include "WPILib.h"

class JakeGyro : public I2C
{
protected:
	static const uint8_t kPowerMgmRegister = 0x3E;
	static const uint8_t kDataFormatRegister = 0x31;
	static const uint8_t kSampleRateDivider = 0x15;
	static const uint8_t kDLPFRegister = 0x16;
	static const uint8_t kTempRegister = 0x1B;
	static const uint8_t kDataRegister = 0x1D;
	static const uint8_t kIntCfg = 0x17;
	static const uint8_t kIntStatus = 0x1A;
	static constexpr double kGsPerLSB = 0.00390625;
	enum PowerCtlFields {kPowerCtl_Link=0x20, kPowerCtl_AutoSleep=0x10, kPowerCtl_Measure=0x08, kPowerCtl_Sleep=0x04};
	enum DataFormatFields {kDataFormat_SelfTest=0x80, kDataFormat_SPI=0x40, kDataFormat_IntInvert=0x20,
		kDataFormat_FullRes=0x08, kDataFormat_Justify=0x04};

public:
	enum Axes {kAxis_X=0x00, kAxis_Y=0x02, kAxis_Z=0x04};
		double temp;
		double XAxis;
		double YAxis;
		double ZAxis;

public:
	explicit JakeGyro(Port port);
	virtual ~JakeGyro();

	// Accelerometer interface
	uint8_t GetReg0();
	virtual double GetX();
	virtual double GetY();
	virtual double GetZ();
	virtual int GetTemp();
	virtual void Update();
	virtual int16_t GetReg(uint8_t regNum);
	virtual double GetAngle();
	void Reset();

	virtual std::string GetSmartDashboardType();
	virtual void Init();

protected:
	//I2C* m_i2c;
private:
	double temperature;
	double AxisX;
	double AxisY;
	double AxisZ;
	double angle;
	double angleBias;
	double lastUpdate;

};
