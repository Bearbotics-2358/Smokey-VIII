#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPILib.h"
#include "ToteDetector.h"
#include "LightRingController.h"
#include "Lifterino.h"

enum AutoState {
	kGrabbing = 0,
	kFindingTote,
	kDrivingToAutoZone,
	kPlacing,
	kBacking,
	kIdle

};

class Smokey_VIII : public IterativeRobot {
public:
	Smokey_VIII(void);
	void RobotInit(void);
	void TeleopInit(void);
	void TeleopPeriodic(void);
	void TestInit(void);
	void TestPeriodic(void);
	void AutonomousInit(void);
	void AutonomousPeriodic(void);

private:
	Joystick a_Joystick;
	Joystick a_Joystick2;
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
	Lifterino a_Lifter;

	PowerDistributionPanel a_PDP;
};

#endif // SMOKEY_VIII_H
