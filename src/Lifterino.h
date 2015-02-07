#ifndef LIFTERINO_H
#define LIFTERINO_H

#include <WPILib.h>

enum LifterinoState {
	kNoTotes = 0,
	kGrip,
	kGripDelay,
	kLift,
	kRelease,
	kIdleWithTote
};

class Lifterino{
public:
	Lifterino(void);
	void Update(Joystick &stick, Joystick &stick2);
	void TestUpdate(Joystick &stick, Joystick &stick2);
	void Reset(void);

private:
	Talon a_Rlifter;
	Talon a_Llifter;
	DoubleSolenoid a_Grip;
	DoubleSolenoid a_Samurai;

	Encoder a_Encoder;
	LifterinoState a_State;

	Timer a_Timer;
};

#endif // LIFTERINO_H
