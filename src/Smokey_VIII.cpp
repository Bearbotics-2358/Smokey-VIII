#include "PREFS.h"

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
	  a_Detectorino(DETECTOR_IP)
{

}


void Smokey_VIII::RobotInit(void)
{

}


void Smokey_VIII::TeleopInit(void)
{

}

void Smokey_VIII::TeleopPeriodic(void)
{
	double slice = a_Joystick.GetY();
	bool processImage = a_Joystick.GetRawButton(1);

	if(processImage) {
		a_Detectorino.CheckForTote(true);
	}

	a_FLmotor.Set(slice);
	SmartDashboard::PutNumber("Current", a_FLmotor.GetOutputCurrent());
	SmartDashboard::PutNumber("Temperature", a_FLmotor.GetTemperature());
}

void Smokey_VIII::TestPeriodic(void)
{

}


START_ROBOT_CLASS(Smokey_VIII);
