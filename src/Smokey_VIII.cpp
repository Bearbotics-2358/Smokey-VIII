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
	  a_Llifter(LEFTLIFTER_PORT)
{

}

Smokey_VIII::~Smokey_VIII(void){

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
	double slicex = a_Joystick.GetX();
	double slice2 = -1 * a_Joystick.GetY();
	a_FLmotor.Set(slice);
	// a_FRmotor.Set(slice2);
	// a_BLmotor.Set(slicex);
	SmartDashboard::PutNumber("Current", a_FLmotor.GetOutputCurrent());
	SmartDashboard::PutNumber("Temperature", a_FLmotor.GetTemperature());
}

void Smokey_VIII::TestPeriodic(void)
{

}


START_ROBOT_CLASS(Smokey_VIII);
