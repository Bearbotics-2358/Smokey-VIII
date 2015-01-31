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
	  a_Encoder(LIFT_ENCODER_PORT_1, LIFT_ENCODER_PORT_2),
	  a_Gyro(GYRO_PORT) {
}

void Smokey_VIII::RobotInit(void) {
	a_Compressor.SetClosedLoopControl(true);
	a_Encoder.SetDistancePerPulse(0.5);
	//a_X = 0.0, a_Y = 0.0, a_Z = 0.0;
}


void Smokey_VIII::TeleopInit(void) {
	a_Encoder.Reset();
}

void Smokey_VIII::TeleopPeriodic(void) {
	// AJM: Uncomment when ready for drive
	// double driveX = a_Joystick.GetX();
	// double driveY = a_Joystick.GetY();
	// a_Drive.MecanumDrive_Cartesian(driveX, driveY, 0.0, 0.0);

	a_Pnu.Update(a_Joystick);

	double slice = a_Joystick.GetY();

	//a_X += x;
	//a_Y += y;
	//a_Z += z;

	//SmartDashboard::PutNumber("Accelerometer X", a_X);
	//SmartDashboard::PutNumber("Accelerometer Y", a_Y);
	//SmartDashboard::PutNumber("Accelerometer Z", a_Z);

	SmartDashboard::PutNumber("Encoder", a_Encoder.GetDistance());

	/*
	if(a_Joystick.GetRawButton(3))
	{
		a_BLmotor.Set(0.25);
		a_FRmotor.Set(-0.25);
	}
	else if(a_Joystick.GetRawButton(2))
	{
		a_BLmotor.Set(-0.25);
		a_FRmotor.Set(0.25);
	}
	else
	{
		a_BLmotor.Set(0);
		a_FRmotor.Set(0);
	}
	*/

	a_FRmotor.Set(slice * 0.125);

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
