#include "Smokey_VIII.h"
#include "Prefs.h"
#include <math.h>

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
  //a_JakeGyro(I2C::kMXP),
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
	//a_Drive.SetInvertedMotor(a_Drive.kRearLeftMotor, true);
	//a_Drive.SetInvertedMotor(a_Drive.kFrontLeftMotor, true);
	a_DriveEncoder.SetDistancePerPulse(((4.0 * M_PI) / 90.0) * (162.0 / 150.0));
	a_DriveEncoder.Reset();
}

void Smokey_VIII::RobotInit(void) {
	a_Compressor.SetClosedLoopControl(true);
	a_Drive.SetExpiration(0.5);
}


void Smokey_VIII::TeleopInit(void) {
	a_Lifter.Reset();
}

void Smokey_VIII::TeleopPeriodic(void) {
	double stickX = a_Joystick.GetX();
	double stickY = a_Joystick.GetY();
	double stickZ = a_Joystick.GetZ();
	if(stickZ < 0 && stickZ > -0.3) {
		stickZ = 0;
	}

	a_Drive.MecanumDrive_Cartesian(stickX, stickY, stickZ, 0.0);
	a_Lifter.Update(a_Joystick, a_Joystick2);
	a_Tongue.TestUpdate(a_Joystick, a_Joystick2);
}

void Smokey_VIII::TestInit(void) {
	a_Lifter.Reset();
	// a_Lifter.SetEnabled(true);
	a_Lifter.SetEnabled(false);
	a_LRC.SetColor(0, 25, 0, 25);
	a_Tongue.lol();
}

void Smokey_VIII::TestPeriodic(void) {
	a_Lifter.TestUpdate(a_Joystick, a_Joystick2);
	a_Tongue.TestUpdate(a_Joystick, a_Joystick2);

	if(a_Joystick.GetRawButton(10)){
		a_Lifter.Reset();
	}

	//a_Tongue.Set(0);

	double stickX = a_Joystick.GetX();
	double stickY = a_Joystick.GetY();
	double stickZ = a_Joystick.GetZ();
	if(stickZ < 0 && stickZ > -0.3) {
		stickZ = 0;
	}

	a_Drive.MecanumDrive_Cartesian(stickX, stickY, stickZ, 0.0);

	//a_DS.SendDouble("Current A", a_PDP.GetCurrent(3));
	//a_DS.SendDouble("Current B", a_PDP.GetCurrent(2));

	SmartDashboard::PutNumber("JoystickZ", a_Joystick.GetZ());
	SmartDashboard::PutNumber("Joystick X", a_Joystick.GetX());
	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(3));
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(2));
	SmartDashboard::PutNumber("drive encoder", a_DriveEncoder.GetDistance());
	SmartDashboard::PutBoolean("compressor", a_Compressor.GetClosedLoopControl());

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
}

void Smokey_VIII::AutonomousPeriodic(void) {
  ToteDetector::Tote tote;
  double toteError = 0.0;
	float targetX = 0;

	SmartDashboard::PutNumber("drive encoder", a_DriveEncoder.GetDistance());

  AutoState nextState = a_AutonState;
	switch (a_AutonState) {
	case kGrabbing: // Extends tongue and retracts it - grabs bin
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
		a_Tongue.UpdateAuto();
		// this state is complete when Tongue state machine reaches kTongueIdle
		if(a_Tongue.GetState() == TongueState::kTongueIdle) {
			// After grabbing all 3 bins, head toward Auto Zone
			if(numOfIterations >= 2) {
				nextState = kLiftBeforeTurn;
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
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
		a_Lifter.AutonUpdate();
		if(a_Lifter.GetAutoState() == LifterinoState::kLift) {
			nextState = kMoveToNext;
		}
		break;

	case kMoveToNext: // Moves the bot about 3 feet right
		// Continue running lifter until completed
		a_Lifter.AutonUpdate();
		a_Drive.MecanumDrive_Cartesian(0.5, 0.0, 0.0, 0.0);
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
		if(a_DriveEncoder.GetDistance() >= targetX)
		{
			nextState = kFindingTote;
		}
		break;

	case kFindingTote: // Vision code implementation needed here
    tote = a_Detectorino.FindTote(true);
    if (tote.present) {
      toteError = (320.0 - tote.x);
      if (fabs(toteError) < 0.1) {
        nextState = kGrabbing;
        a_Tongue.InitAuto();
        numOfIterations++;
      } else {
        if (toteError > 0.5) {
          toteError = 0.5;
        } else if (toteError < -0.5) {
          toteError = -0.5;
        }
        a_Drive.MecanumDrive_Cartesian(toteError, 0.0, 0.0, 0.0);
      }
    } else {
      // Keep driving?
      a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
    }
		break;

	case kLiftBeforeTurn:
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
		a_Lifter.AutonUpdate();
		if(a_Lifter.GetAutoState() == LifterinoState::kLift) {
			nextState = kTurningBot;
		}
		break;

	case kTurningBot: // Turn the bot
		a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0); // Will need to remove later
		a_Tongue.Raise();
		/*
		if(a_JakeGyro.Get() >= -90){
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 1.0, 0.0);
		}
		else
		{*/
		a_DriveEncoder.Reset();
		nextState = kDrivingToAutoZone;
		//}
		break;

	case kDrivingToAutoZone: // Move 6 feet into auto zone
		// TEMPORARY for testing until we get gyro working
		if(a_DriveEncoder.GetDistance() >= -1 * SIX_FEET)
		{
			//a_Drive.MecanumDrive_Cartesian(1.0, 0.0, 0.0, 0.0);
			a_Drive.MecanumDrive_Cartesian(-0.5, 0.0, 0.0, 0.0);
		}
		else
		{
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
			nextState = kBacking;
			a_AutonTimer.Reset();
			a_AutonTimer.Start();
		}
		break;

	case kBacking: // Let go and back away
		a_Lifter.Reset();
		if(a_AutonTimer.Get() <= 3)
		{
			a_Drive.MecanumDrive_Cartesian(0.0, -1.0, 0.0, 0.0);
		}
		else
		{
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
			nextState = kIdle;
		}
		break;

	case kIdle:
		break;
	}
	a_AutonState = nextState;
}

START_ROBOT_CLASS(Smokey_VIII);
