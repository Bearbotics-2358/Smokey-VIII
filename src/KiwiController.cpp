#include "KiwiController.h"
#include "LiveWindow/LiveWindow.h"

// The speed will be passed to the right controller and inverted to the left controller
KiwiController::KiwiController(Talon &Left, Talon &Right)
: LeftC(Left),
  RightC(Right),
  m_speed(0)
{

}

KiwiController::~KiwiController()
{
}

void KiwiController::Set(float speed, uint8_t syncGroup)
{
	RightC.Set(speed);
	LeftC.Set(-speed);
	m_speed = speed;
}

float KiwiController::Get()
{
	return m_speed;
}

void KiwiController::Disable()
{
	// See Talon.Disable()
	RightC.SetRaw(0);
	LeftC.SetRaw(0);
}

void KiwiController::PIDWrite(float output)
{
	Set(output);
}
