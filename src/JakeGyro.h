#pragma once

#include "interfaces/Accelerometer.h"
#include "I2C.h"
#include "LiveWindow/LiveWindowSendable.h"

class JakeGyro : public Accelerometer, public I2C, public LiveWindowSendable
{
protected:
	static const uint8_t kPowerCtlRegister = 0x2D;
	static const uint8_t kDataFormatRegister = 0x31;
	static const uint8_t kDataRegister = 0x1D;
	static const uint8_t kTempRegister = 0x1B;
	static constexpr double kGsPerLSB = 0.00390625;
	enum PowerCtlFields {kPowerCtl_Link=0x20, kPowerCtl_AutoSleep=0x10, kPowerCtl_Measure=0x08, kPowerCtl_Sleep=0x04};
	enum DataFormatFields {kDataFormat_SelfTest=0x80, kDataFormat_SPI=0x40, kDataFormat_IntInvert=0x20,
		kDataFormat_FullRes=0x08, kDataFormat_Justify=0x04};

public:
	enum Axes {kAxis_X=0x00, kAxis_Y=0x02, kAxis_Z=0x04};
	struct AllAxes
	{
		double XAxis;
		double YAxis;
		double ZAxis;
	};

public:
	explicit JakeGyro(Port port);
	virtual ~JakeGyro();

	// Accelerometer interface
	virtual void SetRange(Range range);
	virtual double GetX();
	virtual double GetY();
	virtual double GetZ();
	virtual int GetTemp();

	virtual double GetAcceleration(Axes axis);
	virtual AllAxes GetAccelerations();

	virtual std::string GetSmartDashboardType();
	virtual void InitTable(ITable *subtable);
	virtual void UpdateTable();
	virtual ITable* GetTable();
	virtual void StartLiveWindowMode() {}
	virtual void StopLiveWindowMode() {}

protected:
	//I2C* m_i2c;
private:
	ITable *m_table;
};
