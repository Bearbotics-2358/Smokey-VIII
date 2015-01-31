#include "Smokey_VIII.h"
#include "Prefs.h"

Smokey_VIII::Smokey_VIII(void)
	: a_Joystick(JOYSTICK_PORT),
	  a_FLmotor(FL_PORT),
	  a_FRmotor(FR_PORT),
	  a_BLmotor(BL_PORT),
	  a_BRmotor(BR_PORT),
	  a_Drive(a_FLmotor, a_BLmotor, a_FRmotor, a_BRmotor),
	  a_Tongue(TONGUE_PORT),
	  a_Rlifter(RIGHTLIFTER_PORT),
	  a_Llifter(LEFTLIFTER_PORT),
	  a_Compressor(),
	  a_Pnu(),
	  a_Detectorino(DETECTOR_IP),
	  a_Accel(Accelerometer::kRange_4G),
	  a_Gyro(GYRO_PORT)
{

}


void Smokey_VIII::RobotInit(void) {
	// AJM: Enable when ready for pneumatics
	// a_Compressor.SetClosedLoopControl(true);
}


void Smokey_VIII::TeleopInit(void) {
}

void Smokey_VIII::TeleopPeriodic(void) {
	// AJM: Uncomment when ready for drive
	// double driveX = a_Joystick.GetX();
	// double driveY = a_Joystick.GetY();
	// a_Drive.MecanumDrive_Cartesian(driveX, driveY, 0.0, 0.0);

	double accelX = a_Accel.GetX();
	double accelY = a_Accel.GetY();
	double accelZ = a_Accel.GetZ();

	bool processImage = a_Joystick.GetRawButton(1);

	try {
		if(processImage) {
			a_Detectorino.CheckForTote(true);
		}
	} catch(std::exception &ex) {
		printf("Exception: %s\n", ex.what());
	}
}

void Smokey_VIII::TestPeriodic(void) {
}

START_ROBOT_CLASS(Smokey_VIII);
