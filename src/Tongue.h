#pragma once

#include <WPILib.h>

enum TongueState {
	kExtending = 0,
	kRetracting,
	kTongueIdle
};

class Tongue {
public:
	Tongue(void);
	void Update(Joystick &stick, Joystick &stick2);
	void TestUpdate(Joystick &stick, Joystick &stick2);
	void Extend(bool startingLoop);

private:
	DoubleSolenoid a_TonguePiston;
	Talon a_TongueMotor;
	DigitalInput a_TongueFrontSwitch;
	DigitalInput a_TongueBackSwitch;
	TongueState a_TongueState;
};
