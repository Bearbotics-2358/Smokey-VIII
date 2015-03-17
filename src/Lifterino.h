#ifndef LIFTERINO_H
#define LIFTERINO_H

#include <WPILib.h>
#include "KiwiController.h"
#include "PIDController.h"
#include <Prefs.h>

enum LifterinoState {
	kFindZero = 0,
	kNoTotes,
	kGrip,
	kGripDelay,
	kLift,
	kRelease,
	kIdleWithTote,
	kLower,
	kReleaseDelay,
	kMoveDown
};

class Lifterino {
public:
	Lifterino(void);
	void Update(Joystick &stick, Joystick &stick2);
	void TestUpdate(Joystick &stick, Joystick &stick2);
	void Reset(void);
	void SetEnabled(bool enable);
	void AutonUpdate(void);
	void SetState(LifterinoState stateToSet);
	LifterinoState GetAutoState();
	void SwitchToManual(bool);

private:
	const double TOP_LIFTER_SETPOINT = PREFS_LIFTER_SETPOINT;
	const double BOTTOM_LIFTER_SETPOINT = 0.0;
	const double MOVE_DOWN_SETPOINT = 40.0;

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
	bool enabled;
};

#endif // LIFTERINO_H
