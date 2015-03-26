#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPILib.h"

#include <zmqpp.hpp>

#include "ToteDetector.h"
#include "LightRingController.h"
#include "Lifterino.h"
#include "Tongue.h"
#include "JakeGyro.h"
#include "I2C.h"
#include "RemoteGyro.h"

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
	zmqpp::context a_ZmqCtx;

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

	RemoteGyro a_Gyro;

	LightRingController a_LRC;

	Lifterino a_Lifter;

	PowerDistributionPanel a_PDP;

	Encoder a_DriveEncoder;

	Timer a_AutonTimer;
	AutoState a_AutonState;
};

#endif // SMOKEY_VIII_H
