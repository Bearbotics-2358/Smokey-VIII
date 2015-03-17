#include "Smokey_VIII.h"
#include <Math.h>
#include <Prefs.h>

int numOfIterations = 0;

Smokey_VIII::Smokey_VIII(void)
: a_Joystick(JOYSTICK_PORT),
  a_Joystick2(JOYSTICKTWO_PORT),
  a_FLmotor(FL_PORT),
  a_FRmotor(FR_PORT),
  a_BLmotor(BL_PORT),
  a_BRmotor(BR_PORT),
  a_Drive(a_FLmotor, a_BLmotor, a_FRmotor, a_BRmotor),
  a_Tongue(),
  a_Compressor(),
  a_Detectorino(DETECTOR_IP),
  a_Accel(Accelerometer::kRange_4G),
  a_JakeGyro(I2C::kMXP),
  a_LRC(),
  a_Lifter(),
  a_PDP(),
  a_DS(),
  a_DriveEncoder(DRIVE_ENCODER_PORT_1, DRIVE_ENCODER_PORT_2, true, Encoder::k4X),
  a_AutonTimer(),
  a_AutonState(kGrabbing)
{
	a_Drive.SetInvertedMotor(a_Drive.kRearRightMotor, true);
	a_Drive.SetInvertedMotor(a_Drive.kFrontRightMotor, true);
	// Comment next 2 out for practice bot
	a_Drive.SetInvertedMotor(a_Drive.kRearLeftMotor, true);
	a_Drive.SetInvertedMotor(a_Drive.kFrontLeftMotor, true);
	a_DriveEncoder.SetDistancePerPulse(((4.0 * M_PI) / 90.0) * (162.0 / 150.0));
	a_DriveEncoder.Reset();
}

void Smokey_VIII::RobotInit(void) {
	a_Compressor.SetClosedLoopControl(true);
	a_Drive.SetExpiration(10);
	a_JakeGyro.Init();
}


void Smokey_VIII::TeleopInit(void) {
	a_Lifter.Reset();

	a_JakeGyro.Reset();
	a_JakeGyro.Init();

	a_Tongue.lol();
}

void Smokey_VIII::TeleopPeriodic(void) {
	double stickX = a_Joystick.GetX();
	double stickY = a_Joystick.GetY();
	double stickZ = a_Joystick.GetZ();
	if(stickZ < 0 && stickZ > -0.3) {
		stickZ = 0;
	}
	if(a_Joystick.GetRawButton(7)) {
		a_JakeGyro.Reset();
	}
	a_JakeGyro.Update();

	double gyroAngle = a_JakeGyro.GetAngle();
	gyroAngle = 0;
	a_Drive.MecanumDrive_Cartesian(stickX, stickY, stickZ, gyroAngle);

	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(3));
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(2));


	a_Lifter.Update(a_Joystick, a_Joystick2);
	a_Tongue.Update(a_Joystick, a_Joystick2);
}

void Smokey_VIII::TestInit(void) {
	a_Lifter.Reset();
	// a_Lifter.SetEnabled(true);
	a_Lifter.SetEnabled(false);
	a_LRC.SetColor(0, 25, 0, 25);
	a_Tongue.lol();
	a_JakeGyro.Reset();
	a_JakeGyro.Init();
}

void Smokey_VIII::TestPeriodic(void) {
	a_Lifter.TestUpdate(a_Joystick, a_Joystick2);
	a_Tongue.TestUpdate(a_Joystick, a_Joystick2);
	//a_Lifter.Update(a_Joystick, a_Joystick2);
	if(a_Joystick.GetRawButton(10)){
		a_Lifter.Reset();
	}
	if(a_Joystick.GetRawButton(7)) {
		a_JakeGyro.Reset();
	}
	a_JakeGyro.Update();

	SmartDashboard::PutNumber("Return Temp", a_JakeGyro.GetTemp());
	SmartDashboard::PutNumber("Return X", a_JakeGyro.GetX());
	SmartDashboard::PutNumber("Return Y", a_JakeGyro.GetY());
	SmartDashboard::PutNumber("Return Z", a_JakeGyro.GetZ());
	SmartDashboard::PutNumber("Angle", a_JakeGyro.GetAngle());

	//a_Tongue.Set(0);

	double stickX = a_Joystick.GetX();
	double stickY = a_Joystick.GetY();
	double stickZ = a_Joystick.GetZ();
	if(stickZ < 0 && stickZ > -0.3) {
		stickZ = 0;
	}
	double gyroAngle = a_JakeGyro.GetAngle();
	a_Drive.MecanumDrive_Cartesian(stickX, stickY, stickZ, gyroAngle);

	SmartDashboard::PutNumber("Angle", gyroAngle);

	//a_DS.SendDouble("Current A", a_PDP.GetCurrent(3));
	//a_DS.SendDouble("Current B", a_PDP.GetCurrent(2));

	SmartDashboard::PutNumber("JoystickZ", a_Joystick.GetZ());
	SmartDashboard::PutNumber("Joystick X", a_Joystick.GetX());
	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(3));
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(2));
	SmartDashboard::PutNumber("drive encoder", a_DriveEncoder.GetDistance());
	SmartDashboard::PutBoolean("compressor", a_Compressor.GetClosedLoopControl());
	SmartDashboard::PutNumber("DrimmX", a_JakeGyro.GetX());
	SmartDashboard::PutNumber("DrimmY", a_JakeGyro.GetY());
	SmartDashboard::PutNumber("DrimmZ", a_JakeGyro.GetZ());

	bool result = false;
	if(a_Joystick.GetRawButton(3)){
		result = a_LRC.SetColor(0, 60, 0, 0);
		SmartDashboard::PutBoolean("I2C result", result);
	}else if(a_Joystick.GetRawButton(4)){
		result = a_LRC.SetColor(0, 0, 60, 0);
		SmartDashboard::PutBoolean("I2C result", result);
	}else if(a_Joystick.GetRawButton(5)){
		result = a_LRC.SetColor(0, 0, 0, 60);
		SmartDashboard::PutBoolean("I2C result", result);
	}else if(a_Joystick.GetRawButton(6)){
		result = a_LRC.SetColor(0, 255, 255, 255);
		SmartDashboard::PutBoolean("I2C result", result);
	}

	/*
		bool processImage = a_Joystick.GetRawButton(1);

		try {
			if(processImage) {
				// a_Detectorino.CheckForTote(true);
			}
		} catch(std::exception &ex) {
			printf("Exception: %s\n", ex.what());
		}
	 */
}

void Smokey_VIII::AutonomousInit(void) {
	a_Lifter.Reset();
	a_Lifter.SetEnabled(false);
	a_Tongue.InitAuto();
	a_LRC.SetColor(0,36,72,72);
	a_DriveEncoder.Reset();
	a_JakeGyro.Reset();
}

void Smokey_VIII::AutonomousPeriodic(void) {
	float targetX = 0;
	double gyroAngle = a_JakeGyro.GetAngle();

	AutoState nextState = a_AutonState;

	SmartDashboard::PutNumber("drive encoder", a_DriveEncoder.GetDistance());

	switch (a_AutonState) {

	case kGrabbing: // Extends tongue and retracts it - grabs bin
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle);
		a_Tongue.UpdateAuto();
		// this state is complete when Tongue state machine reaches kTongueIdle
		if(a_Tongue.GetState() == TongueState::kTongueIdle) {
			// After grabbing all 3 bins, head toward Auto Zone
			if(numOfIterations >= 2) {
				nextState = kGrabBeforeTurn;
				a_Lifter.SetState(LifterinoState::kRelease);
			} else {
				nextState = kLifting;

				// prepare to lift next bin
				//a_Lifter.Reset();
				if(numOfIterations > 0) {
					a_Lifter.SetState(LifterinoState::kRelease);
				}
				//a_Lifter.SetEnabled(false);
			}
		}
		break;

	case kLifting: // Lifts a grabbed bin
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle);
		a_Lifter.AutonUpdate();
		if(a_Lifter.GetAutoState() == LifterinoState::kLift) {
			nextState = kMoveToNext;
		}
		break;

	case kMoveToNext: // Moves the bot about 3 feet right
		// Continue running lifter until completed
		a_Lifter.AutonUpdate();
		a_Drive.MecanumDrive_Cartesian(0.5, 0.0, 0.0, gyroAngle);
		switch(numOfIterations) {
		case 0:
			targetX = THREE_FEET;
			break;

		case 1:
			targetX = SIX_FEET;
			break;

		case 2:
			targetX = SIX_FEET;
			break;
		}
		if(a_DriveEncoder.GetDistance() >= targetX) {
			nextState = kFindingTote;
		}
		break;

	case kFindingTote: // Vision code implementation needed here
		a_Drive.MecanumDrive_Cartesian(0, 0.0, 0.0, gyroAngle);
		if(true || a_Detectorino.CheckForTote(true)) {
			nextState = kGrabbing;
			a_Tongue.InitAuto();
			numOfIterations++;
		}
		break;

	case kGrabBeforeTurn:
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle);
		a_Lifter.AutonUpdate();
		if(a_Lifter.GetAutoState() == LifterinoState::kLift) {
			nextState = kTurningBot;
		}
		break;

	case kTurningBot: // Turn the bot
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle); // Will need to remove later
		a_Tongue.Raise();
		if(a_JakeGyro.GetX() <= 90){
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, -.5, 0);
		} else {
		a_DriveEncoder.Reset();
		nextState = kDrivingToAutoZone;
		}
		break;

	case kDrivingToAutoZone: // Move 6 feet into auto zone
		// TEMPORARY for testing until we get gyro working
		if(a_DriveEncoder.GetDistance() >= -1 * SIX_FEET)
		{
			a_Drive.MecanumDrive_Cartesian(0.0, 0.5, 0.0, gyroAngle);
			//a_Drive.MecanumDrive_Cartesian(-0.5, 0.0, 0.0, 0.0);
		}
		else
		{
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle);
			nextState = kBacking;
			a_AutonTimer.Reset();
			a_AutonTimer.Start();
		}
		break;

	case kBacking: // Let go and back away
		a_Lifter.Reset();
		if(a_AutonTimer.Get() <= 3)
		{
			a_Drive.MecanumDrive_Cartesian(0.5, 0.0, 0.0, gyroAngle);
		}
		else
		{
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle);
			nextState = kIdle;
		}
		break;

	case kIdle:
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, gyroAngle);
		break;
	}
	a_AutonState = nextState;
}

START_ROBOT_CLASS(Smokey_VIII);
