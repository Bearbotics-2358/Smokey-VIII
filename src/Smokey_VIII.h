#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPILib.h"
#include "ToteDetector.h"
#include "LightRingController.h"
#include "Lifterino.h"
#include "DumbDashboard.h"
#include "Tongue.h"
#include "JakeGyro.h"
#include "I2C.h"

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
	void AutonomousPeriodic(void);

private:
	Joystick a_Joystick;
	Joystick a_Joystick2;
	CANTalon   a_FLmotor;
	CANTalon   a_FRmotor;
	CANTalon   a_BLmotor;
	CANTalon   a_BRmotor;
	RobotDrive a_Drive;

	Tongue a_Tongue;

	Compressor   a_Compressor;

	ToteDetector a_Detectorino;

	BuiltInAccelerometer a_Accel;

	JakeGyro a_JakeGyro;

	LightRingController a_LRC;

	Lifterino a_Lifter;

	PowerDistributionPanel a_PDP;

	DumbDashboard a_DS;
	Encoder a_DriveEncoder;

	Timer a_AutonTimer;
	AutoState a_AutonState;
};

#endif // SMOKEY_VIII_H
