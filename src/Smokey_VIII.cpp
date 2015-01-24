#include "PREFS.h"
#include "SmokeyPnu.h"
#include "Smokey_VIII.h"

Smokey_VIII::Smokey_VIII(void)
	: a_Gyro(GYRO_PORT),
	  a_Joystick(JOYSTICK_PORT),
	  a_FLmotor(FL_PORT),
	  a_FRmotor(FR_PORT),
	  a_BLmotor(BL_PORT),
	  a_Tongue(TONGUE_PORT),
	  a_Rlifter(RIGHTLIFTER_PORT),
	  a_Llifter(LEFTLIFTER_PORT),
	  a_Detectorino(DETECTOR_IP),
	  a_Compressor(),
	  a_Pnu(),
	  a_Accel(Accelerometer::kRange_4G)
{

}


void Smokey_VIII::RobotInit(void)
{
	a_Compressor.SetClosedLoopControl(false);
}


void Smokey_VIII::TeleopInit(void)
{
}

void Smokey_VIII::TeleopPeriodic(void)
{
	// a_Pnu.Update(a_Joystick);

	double slice = a_Joystick.GetY();
	double x = a_Accel.GetX();
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
	if(a_Joystick.GetRawButton(2)) {
		a_FRmotor.Set(slice);
	} else {
		a_FRmotor.Set(slice * 0.125);
	}

	bool processImage = a_Joystick.GetRawButton(1);

	if(processImage) {
		a_Detectorino.CheckForTote(true);
	}
}

void Smokey_VIII::TestPeriodic(void)
{

}


START_ROBOT_CLASS(Smokey_VIII);
