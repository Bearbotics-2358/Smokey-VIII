#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPILib.h"
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
	Joystick a_Joystick;

	CANTalon   a_FLmotor;
	CANTalon   a_FRmotor;
	CANTalon   a_BLmotor;
	CANTalon   a_BRmotor;
	RobotDrive a_Drive;

	Talon a_Tongue;
	Talon a_Rlifter;
	Talon a_Llifter;

	Compressor   a_Compressor;
	SmokeyPnu    a_Pnu;

	ToteDetector a_Detectorino;

	BuiltInAccelerometer a_Accel;
	Gyro a_Gyro;
};

#endif // SMOKEY_VIII_H
