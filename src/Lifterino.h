#ifndef LIFTERINO_H
#define LIFTERINO_H

#include <WPILib.h>
#include "KiwiController.h"
#include "PIDController.h"

enum LifterinoState {
	kFindZero = 0,
	kNoTotes,
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
	void AutonUpdate(void);

private:
	const double TOP_LIFTER_SETPOINT = 65.0;
	const double BOTTOM_LIFTER_SETPOINT = 3.0;

	Talon a_Rlifter;
	Talon a_Llifter;
	DoubleSolenoid a_Grip;

	Encoder a_Encoder;
	LifterinoState a_State;
	LifterinoState a_AutoState;

	Timer a_Timer;

	DigitalInput a_LifterSwitch;

	KiwiController a_LifterC;

	PIDController a_PID;

	//operating on field 3/7 When slider
	float P = 0.1;
	float I = 0.0;
	float D = 0.0;
	bool a_enabled;
};

#endif // LIFTERINO_H
