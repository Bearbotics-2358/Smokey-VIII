#include "Lifterino.h"
#include "Prefs.h"
#include "PIDController.h"

Lifterino::Lifterino()
	:a_Rlifter(RIGHTLIFTER_PORT),
	 a_Llifter(LEFTLIFTER_PORT),
	 a_Grip(GRIPPER_EXTEND_SOLENOID, GRIPPER_RETRACT_SOLENOID),
	 a_Samurai(SAMURAI_SWORD_RAISE, SAMURAI_SWORD_LOWER),
	 a_Encoder(LIFT_ENCODER_PORT_1, LIFT_ENCODER_PORT_2, true, Encoder::k4X),
	 a_State(kNoTotes),
	 a_Timer(),
	 a_LifterSwitch(LIFTER_SWITCH_PORT),
	 a_LifterC(a_Llifter, a_Rlifter),
	 a_PID(P, I, D, &a_Encoder, &a_LifterC, 0.05)
{
	SetEnabled(false);
	a_Encoder.SetPIDSourceParameter(PIDSource::kDistance);
	a_Encoder.SetDistancePerPulse(1);

	a_PID.SetOutputRange(-1.0, 1.0);
	a_PID.SetInputRange(-1000, 1000);




}

void Lifterino::Update(Joystick &stick, Joystick &stick2) {
	bool liftButton = stick.GetRawButton(1);
	bool pnuExtendButton = stick2.GetRawButton(3);
	bool pnuRetractButton = stick2.GetRawButton(2);

	if(!a_enabled) {
		a_LifterC.Set(-1 * stick2.GetY());
		if(pnuExtendButton) {
				a_Grip.Set(DoubleSolenoid::kForward);
			} else if(pnuRetractButton) {
				a_Grip.Set(DoubleSolenoid::kReverse);
			}
	}

	// a_Rlifter.Set(-1 * stick2.GetY());
	// a_Llifter.Set(stick2.GetY());


	if(stick2.GetRawButton(1)){
		SetEnabled(true);
		a_PID.SetSetpoint(SmartDashboard::GetNumber("soke"));
	}

	LifterinoState nextState = a_State;
	SmartDashboard::PutNumber("Encoder Value", a_Encoder.GetDistance());
	SmartDashboard::PutBoolean("Lifter Switch", a_LifterSwitch.Get());
	SmartDashboard::PutNumber("Lifter Speed", a_PID.Get());
	SmartDashboard::PutNumber("PID Error", a_PID.GetError());
	SmartDashboard::PutNumber("KiwiSpeed", a_LifterC.Get());
	// SmartDashboard::PutNumber("P", P);
	// SmartDashboard::PutNumber("I", I);
	// SmartDashboard::PutNumber("D", D);

	P = SmartDashboard::GetNumber("P");
	I = SmartDashboard::GetNumber("I");
	D = SmartDashboard::GetNumber("D");

	a_PID.SetPID(P, I, D);

	if(!a_LifterSwitch.Get()){
				a_Encoder.Reset();
	}

// State Machine
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

void Lifterino::SetEnabled(bool enable)
{
	a_enabled = enable;
	(enable) ? a_PID.Enable() : a_PID.Disable();
}


