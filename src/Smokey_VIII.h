#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPILib.h"
#include "ToteDetector.h"
#include "LightRingController.h"

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


	Compressor   a_Compressor;


	// ToteDetector a_Detectorino;

	BuiltInAccelerometer a_Accel;

	Gyro a_Gyro;

	LightRingController a_LRC;
};

#endif // SMOKEY_VIII_H
