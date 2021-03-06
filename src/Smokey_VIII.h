#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPILib.h"
#include "ToteDetector.h"
#include "LightRingController.h"
#include "Lifterino.h"
#include "DumbDashboard.h"
#include "JakeGyro.h"
#include "I2C.h"
#include "Rollers.h"

enum AutoState {
	kGrabbing = 0,
	kFindingTote,
	kDrivingToAutoZone,
	kBacking,
	kIdle,
	kGrabBeforeTurn,
	kTurningBot,
	kLifting,
	kMoveToNext

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
	void AutonomousPeriodicFull(void);
	void AutonomousPeriodic(void);
	void AutonomousPeriodicSimple(void);

private:
	Joystick a_Joystick;
	Joystick a_Joystick2;
	CanTalonSRX   a_FLmotor;
	CanTalonSRX   a_FRmotor;
	CanTalonSRX   a_BLmotor;
	CanTalonSRX   a_BRmotor;
	RobotDrive a_Drive;

	Compressor   a_Compressor;

	ToteDetector a_Detectorino;

	BuiltInAccelerometer a_Accel;

	JakeGyro a_JakeGyro;

	LightRingController a_LRC;

	Lifterino a_Lifter;

	PowerDistributionPanel a_PDP;

	Encoder a_DriveEncoder;

	Timer a_AutonTimer;
	AutoState a_AutonState;
	Rollers a_Rollers;
};

#endif // SMOKEY_VIII_H
