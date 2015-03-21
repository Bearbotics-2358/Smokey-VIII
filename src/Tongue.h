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
	void Raise();
	void InitAuto();
	void UpdateAuto();
	TongueState GetState();
	void lol();
	void SetEnabled(bool);
	void MotorSafeFeed(void);

private:
	const double FORWARD_SPEED = -1;
	const double BACKWARD_SPEED = 0.5;
	DoubleSolenoid a_TonguePiston;
	Talon a_TongueMotor;
	DigitalInput a_TongueFrontSwitch;
	DigitalInput a_TongueBackSwitch;
	TongueState a_TongueState;
	TongueState a_TongueTeleopState;
	bool enabled;

	Timer a_Timer;
	bool a_timer_running;
};
