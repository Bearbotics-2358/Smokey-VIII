#ifndef SMOKEY_PNU_H
#define SMOKEY_PNU_H

#include <WPILib.h>

class SmokeyPnu
{
public:
	SmokeyPnu(void);

	void Update(Joystick &input);

private:
	Solenoid a_extend;
	Solenoid a_retract;
};
#endif // _KIWIPNU_H_
