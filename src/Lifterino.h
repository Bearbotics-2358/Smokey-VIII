#ifndef LIFTERINO_H
#define LIFTERINO_H

#include <WPILib.h>

enum LifterinoState {
	kNoTotes = 0,
	kGrip,
	kGripDelay,
	kLift,
	kIdleWithTote
};

class Lifterino{
public:
	Lifterino(void);
	void Update(Joystick &stick);

private:
	Talon a_Rlifter;
	Talon a_Llifter;

	DoubleSolenoid a_RGripper;
	DoubleSolenoid a_LGripper;

	Encoder a_Encoder;
	LifterinoState a_State;

	Timer a_Timer;
};

#endif // LIFTERINO_H
