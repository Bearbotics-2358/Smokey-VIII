#pragma once

#include <WPIlib.h>
#include "Prefs.h"

class Rollers {
public:
	Rollers(void);
	void Update(Joystick &stick, Joystick &stick2);
	void TestUpdate(Joystick &stick, Joystick &stick2);

private:
	Talon a_LRoller;
	Talon a_RRoller;

	DoubleSolenoid a_RollerSolenoid;

};
