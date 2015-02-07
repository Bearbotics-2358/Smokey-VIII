#include "Smokey_VIII.h"
#include "Prefs.h"


Smokey_VIII::Smokey_VIII(void)
: a_Joystick(JOYSTICK_PORT),
  a_Joystick2(JOYSTICKTWO_PORT),
  a_FLmotor(FL_PORT),
  a_FRmotor(FR_PORT),
  a_BLmotor(BL_PORT),
  a_BRmotor(BR_PORT),
  a_Drive(a_FLmotor, a_BLmotor, a_FRmotor, a_BRmotor),
  a_Tongue(TONGUE_PORT),
  a_Compressor(),
  // a_Detectorino(DETECTOR_IP),
  a_Accel(Accelerometer::kRange_4G),
  a_Gyro(GYRO_PORT),
  a_LRC(),
  a_Lifter()
{
	a_Drive.SetInvertedMotor(a_Drive.kRearRightMotor, true);
	a_Drive.SetInvertedMotor(a_Drive.kFrontRightMotor, true);
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
	a_Drive.MecanumDrive_Cartesian(stickX, stickY, stickZ, 0.0);

	a_Lifter.Update(a_Joystick, a_Joystick2);

	a_Tongue.Set(0);
}

void Smokey_VIII::TestInit(void) {
	a_Lifter.Reset();
}

void Smokey_VIII::TestPeriodic(void) {
	a_Lifter.TestUpdate(a_Joystick, a_Joystick2);

	/*
		bool result = false;
		if(a_Joystick.GetRawButton(3)){
			result = a_LRC.SetColor(0, 255, 0, 0);
			SmartDashboard::PutBoolean("I2C result", result);
		}else if(a_Joystick.GetRawButton(4)){
			result = a_LRC.SetColor(0, 0, 255, 0);
			SmartDashboard::PutBoolean("I2C result", result);
		}else if(a_Joystick.GetRawButton(5)){
			result = a_LRC.SetColor(0, 0, 0, 255);
			SmartDashboard::PutBoolean("I2C result", result);
		}
	 */

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
}

void Smokey_VIII::AutonomousPeriodic(void) {

}

START_ROBOT_CLASS(Smokey_VIII);
