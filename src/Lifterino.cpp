#include "Lifterino.h"
#include "Prefs.h"


//Pneumatics ports 0 and 1 are blue, 2 and 3 are white- blue is extend (clamp) and white is retract
Lifterino::Lifterino()
	:a_Rlifter(RIGHTLIFTER_PORT),
	 a_Llifter(LEFTLIFTER_PORT),
	 a_Grip(GRIPPER_EXTEND_SOLENOID, GRIPPER_RETRACT_SOLENOID),
	 a_Samurai(SAMURAI_SWORD_RAISE, SAMURAI_SWORD_LOWER),
	 a_Encoder(LIFT_ENCODER_PORT_1, LIFT_ENCODER_PORT_2),
	 a_State(kNoTotes),
	 a_Timer(){
	a_Encoder.SetDistancePerPulse(0.5);
	a_Encoder.SetReverseDirection(true);

}

void Lifterino::Update(Joystick &stick, Joystick &stick2) {
	bool liftButton = stick.GetRawButton(1);

	LifterinoState nextState = a_State;
	SmartDashboard::PutNumber("Encoder Value", a_Encoder.Get());

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
		nextState = kGripDelay;
		break;

	case kGripDelay:
		if(a_Timer.HasPeriodPassed(2)){
			nextState = kLift;
		}
		break;

	case kLift:
		a_Llifter.Set(0.1);
		a_Rlifter.Set(-0.1);
		if(a_Encoder.Get() >= 6){
			nextState = kIdleWithTote;
		}
		break;

	case kIdleWithTote:
		a_Llifter.Set(0);
		a_Rlifter.Set(0);
		break;
	}

	a_State = nextState;
}

void Lifterino::TestUpdate(Joystick &stick, Joystick &stick2) {
	bool pnuExtendButton = stick2.GetRawButton(3);
	bool pnuRetractButton = stick2.GetRawButton(2);
	bool samuraiRaiseButton = stick.GetRawButton(5);
	bool samuraiLowerButton = stick.GetRawButton(4);

	a_Rlifter.Set(-1 * stick2.GetY());
	a_Llifter.Set(stick2.GetY());

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
	a_Encoder.Reset();
	a_State = kNoTotes;

}
