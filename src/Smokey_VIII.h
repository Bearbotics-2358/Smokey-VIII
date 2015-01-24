#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPIlib.h"
#include "ToteDetector.h"
#include "SmokeyPnu.h"

class Smokey_VIII : public IterativeRobot {
public:
	Smokey_VIII(void);
	void RobotInit(void);
	void TeleopInit(void);
	void TeleopPeriodic(void);
	void TestPeriodic(void);

private:
	Gyro a_Gyro;
	Joystick a_Joystick;
	CANTalon a_FLmotor;
	CANTalon a_FRmotor;
	CANTalon a_BLmotor;
	Talon a_Tongue;
	Talon a_Rlifter;
	Talon a_Llifter;
	ToteDetector a_Detectorino;
	Compressor a_Compressor;
	SmokeyPnu a_Pnu;
	BuiltInAccelerometer a_Accel;
};

#endif // SMOKEY_VIII_H
