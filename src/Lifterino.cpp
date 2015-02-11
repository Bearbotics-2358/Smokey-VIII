#include "Lifterino.h"
#include "Prefs.h"

Lifterino::Lifterino()
	:a_Rlifter(RIGHTLIFTER_PORT),
	 a_Llifter(LEFTLIFTER_PORT),
	 a_Grip(GRIPPER_EXTEND_SOLENOID, GRIPPER_RETRACT_SOLENOID),
	 a_Samurai(SAMURAI_SWORD_RAISE, SAMURAI_SWORD_LOWER),
	 a_Encoder(LIFT_ENCODER_PORT_1, LIFT_ENCODER_PORT_2, true, Encoder::k4X),
	 a_State(kNoTotes),
	 a_Timer(),
	 a_LifterSwitch(LIFTER_SWITCH_PORT),
	 a_LifterC(a_Llifter, a_Rlifter)
{
	a_Encoder.SetDistancePerPulse(0.25);

}

void Lifterino::Update(Joystick &stick, Joystick &stick2) {
	bool liftButton = stick.GetRawButton(1);

	a_LifterC.Set(-1 * stick2.GetY());
	// a_Rlifter.Set(-1 * stick2.GetY());
	// a_Llifter.Set(stick2.GetY());


	LifterinoState nextState = a_State;
	SmartDashboard::PutNumber("Encoder Value", a_Encoder.GetRaw());

	SmartDashboard::PutBoolean("Lifter Switch", a_LifterSwitch.Get());
	if(!a_LifterSwitch.Get()){
				a_Encoder.Reset();
		}

	switch(a_State){
	case kNoTotes:
		if(liftButton){
			nextState = kGrip;
		}
		break;

	case kGrip:
		a_Grip.Set(DoubleSolenoid::kForward);
		a_Timer.Reset();
		a_Timer.Start();
		nextState = kGripDelay;
		break;

	case kRelease:
		a_Grip.Set(DoubleSolenoid::kReverse);
		a_Timer.Reset();
		a_Timer.Start();
		nextState = kReleaseDelay;
		break;

	case kGripDelay:
		if(a_Timer.Get() >= 1 ){
			nextState = kLift;
		}
		break;

	case kLift:
		if(a_Encoder.GetRaw() >= 215){
					nextState = kIdleWithTote;
				}
		a_LifterC.Set(0.5);
		// a_Llifter.Set(-0.5);
		// a_Rlifter.Set(0.5);

		break;

	case kIdleWithTote:
		a_LifterC.Set(0);
		// a_Llifter.Set(0);
		// a_Rlifter.Set(0);
		if(liftButton){
			nextState = kRelease;
		}
		break;

	case kLower:
		if(a_Encoder.GetRaw() == 0){
					nextState = kGrip;
				}
		a_LifterC.Set(-0.1);
		// a_Llifter.Set(0.1);
		// a_Rlifter.Set(-0.1);

		break;

	case kReleaseDelay:
		if(a_Timer.Get() >= 1){
			nextState = kLower;
		}
	}

	a_State = nextState;
}

void Lifterino::TestUpdate(Joystick &stick, Joystick &stick2) {
	SmartDashboard::PutNumber("Encoder Value", a_Encoder.Get());
	SmartDashboard::PutBoolean("Lifter Switch", a_LifterSwitch.Get());
	bool pnuExtendButton = stick2.GetRawButton(3);
	bool pnuRetractButton = stick2.GetRawButton(2);
	bool samuraiRaiseButton = stick.GetRawButton(5);
	bool samuraiLowerButton = stick.GetRawButton(4);

	if(!a_LifterSwitch.Get()){
			a_Encoder.Reset();
	}

	a_LifterC.Set(-1 * stick2.GetY());
	// a_Rlifter.Set(-1 * stick2.GetY());
	// a_Llifter.Set(stick2.GetY());

	if(pnuExtendButton) {
		a_Grip.Set(DoubleSolenoid::kForward);
	} else if(pnuRetractButton) {
		a_Grip.Set(DoubleSolenoid::kReverse);
	}

	if(samuraiRaiseButton) {
		a_Samurai.Set(DoubleSolenoid::kForward);
	} else if(samuraiLowerButton) {
		a_Samurai.Set(DoubleSolenoid::kReverse);
	}
}

void Lifterino::Reset(void) {
	a_State = kNoTotes;
	a_Grip.Set(DoubleSolenoid::kReverse);

}


