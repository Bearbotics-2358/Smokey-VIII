#include "SmokeyPnu.h"
#include "Prefs.h"

SmokeyPnu::SmokeyPnu(void)
	: a_extend(PISTON_EXTEND_SOLENOID),
	a_retract(PISTON_RETRACT_SOLENOID)
{

}


void SmokeyPnu::Update(Joystick &input)
{
	bool inputExtendButton = input.GetRawButton(6);
	bool inputRetractButton = input.GetRawButton(7);

	if(inputExtendButton)
	{
		a_extend.Set(true);
		a_retract.Set(false);
	}
	else if(inputRetractButton)
	{
		a_extend.Set(false);
		a_retract.Set(true);
	}
}
