#ifndef LIFTERINO_H
#define LIFTERINO_H

#include <WPILib.h>
#include "KiwiController.h"
#include "PIDController.h"

enum LifterinoState {
	kNoTotes = 0,
	kGrip,
	kGripDelay,
	kLift,
	kRelease,
	kIdleWithTote,
	kLower,
	kReleaseDelay
};

class Lifterino {
public:
	Lifterino(void);
	void Update(Joystick &stick, Joystick &stick2);
	void TestUpdate(Joystick &stick, Joystick &stick2);
	void Reset(void);
	void SetEnabled(bool enable);

private:
	Talon a_Rlifter;
	Talon a_Llifter;
	DoubleSolenoid a_Grip;
	DoubleSolenoid a_Samurai;

	Encoder a_Encoder;
	LifterinoState a_State;

	Timer a_Timer;

	DigitalInput a_LifterSwitch;

	KiwiController a_LifterC;

	PIDController a_PID;

	float P = 0.05;
	float I = 0.001;
	float D = 0;
	bool a_enabled;
};

#endif // LIFTERINO_H
