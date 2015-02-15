#pragma once

#include <WPILib.h>

class Tongue {
public:
	Tongue(void);
	void Update(Joystick &stick, Joystick &stick2);
	void TestUpdate(Joystick &stick, Joystick &stick2);

private:
	DoubleSolenoid a_TonguePiston;
	Talon a_TongueMotor;

};
