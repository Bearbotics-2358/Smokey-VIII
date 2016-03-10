#include "KiwiController.h"
#include "LiveWindow/LiveWindow.h"

// The speed will be passed to the right controller and inverted to the left controller
ShifterController::ShifterController(Talon &Left, Talon &Right)
: LeftC(Left),
  RightC(Right),
  m_speed(0)
{

}

ShifterController::~ShifterController()
{
}

void ShifterController::Set(float speed, uint8_t syncGroup)
{
	RightC.Set(speed);
	LeftC.Set(-speed);
	m_speed = speed;
}

float ShifterController::Get()
{
	return m_speed;
}

void ShifterController::Disable()
{
	// See Talon.Disable()
	RightC.SetRaw(0);
	LeftC.SetRaw(0);
}

void ShifterController::PIDWrite(float output)
{
	Set(output);
}
