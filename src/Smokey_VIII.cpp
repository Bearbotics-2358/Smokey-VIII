#include "Smokey_VIII.h"
#include "Prefs.h"

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
  // a_Detectorino(DETECTOR_IP),
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



}

void Smokey_VIII::TestInit(void) {
	a_Lifter.Reset();
	// a_Lifter.SetEnabled(true);
	a_Lifter.SetEnabled(false);

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

	a_DS.SendDouble("Current A", a_PDP.GetCurrent(3));
	a_DS.SendDouble("Current B", a_PDP.GetCurrent(2));

	SmartDashboard::PutNumber("JoystickZ", a_Joystick.GetZ());
	SmartDashboard::PutNumber("Joystick X", a_Joystick.GetX());
	SmartDashboard::PutNumber("Current A", a_PDP.GetCurrent(3));
	SmartDashboard::PutNumber("Current B", a_PDP.GetCurrent(2));
	SmartDashboard::PutNumber("drive encoder", a_DriveEncoder.GetRaw());
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
	// a_Lifter.SetEnabled(true);
	a_Lifter.SetEnabled(false);

}

void Smokey_VIII::AutonomousPeriodic(void) {
	switch (a_AutonState) {

	case kGrabbing: // Extends tongue and retracts it- grabs bin
		if(numOfIterations == 2) {
			a_Tongue.Extend(true);
			a_AutonState = kTurningBot;
		}else {
			a_Tongue.Extend(true);
			a_AutonState = kLifting;
		}
		break;

	case kLifting: // Lifts a grabbed bin
		a_Lifter.AutonUpdate();
		a_AutonState = kMoveToNext;
		break;

	case kMoveToNext: // Moves the bot about 3 feet right
		a_Drive.MecanumDrive_Cartesian(1.0, 0.0, 0.0, 0.0);
		if(a_DriveEncoder.GetRaw() >= 270)
		{
			a_AutonState = kFindingTote;
		}
		break;

	case kFindingTote: // Vision code implementation needed here
		//Slow down until the tote is detected, stop when it is
		a_AutonState = kGrabbing;
		numOfIterations ++;
		a_DriveEncoder.Reset();
		break;

	case kTurningBot: // Turn the bot
		a_Tongue.Raise();
		/*
		if(a_JakeGyro.Get() >= -90){
			a_Drive.MecanumDrive_Cartesian(0.0, 0.0, 1.0, 0.0);
		}
		else
		{*/
		a_DriveEncoder.Reset();
		a_AutonState = kDrivingToAutoZone;
		//}
		break;

	case kDrivingToAutoZone: // Move 6 feet into auto zone
		if(a_DriveEncoder.GetRaw() <= 540)
		{
			a_Drive.MecanumDrive_Cartesian(1.0, 0.0, 0.0, 0.0);
		}
		else
		{
			a_AutonState = kBacking;
			a_AutonTimer.Reset();
			a_AutonTimer.Start();
		}
		break;

	case kBacking: // Let go and back away
		a_Lifter.Reset();
		if(a_AutonTimer.Get() <= 1)
		{
			a_Drive.MecanumDrive_Cartesian(0.0, -1.0, 0.0, 0.0);
		}
		else
		{
		a_AutonState = kIdle;
		}
		break;

	case kIdle:
		break;
	}
}

START_ROBOT_CLASS(Smokey_VIII);
