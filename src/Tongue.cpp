#include <Prefs.h>
#include "Tongue.h"
#include "Utilities.h"

Tongue::Tongue()
: a_TonguePiston(TONGUE_EXTEND_PORT, TONGUE_RETRACT_PORT),
  a_TongueMotor(TONGUE_PORT),
  a_TongueFrontSwitch(TONGUE_SWITCH_PORT),
  a_TongueBackSwitch(TONGUE_BACK_SWITCH_PORT),
  a_TongueState(kTongueIdle),
  a_TongueTeleopState(kTongueIdle)
{
	a_TonguePiston.Set(DoubleSolenoid::kForward);
	enabled = true;
}

void Tongue::Update(Joystick &stick, Joystick &stick2) {
	if(stick.GetRawButton(4)) {
		SetEnabled(false);
	} else if(stick.GetRawButton(3)) {
		SetEnabled(true);
	}

	if(enabled) {
		int tongue_back_int = 0;
		int tongue_front_int = 0;

		if(a_TongueTeleopState == kTongueIdle && stick.GetRawButton(2)) {
			a_TongueTeleopState = kExtending;
		}

		tongue_back_int = (int)a_TongueBackSwitch.Get();
		tongue_front_int = (int)a_TongueFrontSwitch.Get();

		TongueState nextState = a_TongueTeleopState;
		switch (a_TongueTeleopState) {
		case kExtending:
			a_TongueMotor.Set(-0.5);

			if(tongue_back_int == 0) {
				nextState = kRetracting;
			}
			break;

		case kRetracting:
			a_TongueMotor.Set(0.5);

			if(tongue_front_int == 0) {
				nextState = kTongueIdle;
				// next state may never run, so stop motor here
				a_TongueMotor.Set(0.0);
			}
			break;

		case kTongueIdle:
			a_TongueMotor.Set(0.0);
			break;
		}
		a_TongueTeleopState = nextState;
	} else {
		double ForwardSpeed = GetSmartDashboardNumber("TongueForward", -1);
		double BackwardSpeed = GetSmartDashboardNumber("TongueBackward", 0.5);

		if(stick.GetRawButton(11) && a_TonguePiston.Get() == DoubleSolenoid::kForward) {
			a_TongueMotor.Set(ForwardSpeed);
		}else if(stick.GetRawButton(12)) {
			a_TongueMotor.Set(BackwardSpeed);
		}else {
			a_TongueMotor.Set(0.0);
		}
	}
	if(!a_TongueFrontSwitch.Get())
	{
		if(stick2.GetRawButton(4)) {
			a_TonguePiston.Set(DoubleSolenoid::kForward);
		} else if(stick2.GetRawButton(5)) {
			a_TonguePiston.Set(DoubleSolenoid::kReverse);
		}
	}

}

void Tongue::Raise() {
	a_TonguePiston.Set(DoubleSolenoid::kForward);
}

void Tongue::InitAuto()
{
	a_TongueState = kExtending;
	lol();
}

void Tongue::UpdateAuto()
{
	int tongue_back_int = 0;
	int tongue_front_int = 0;

	if(a_TongueState == kTongueIdle) {
		a_TongueState = kExtending;
	}

	SmartDashboard::PutBoolean("Tongue Back Switch", a_TongueBackSwitch.Get());
	SmartDashboard::PutBoolean("Tongue Front Switch", a_TongueFrontSwitch.Get());

	tongue_back_int = (int)a_TongueBackSwitch.Get();
	tongue_front_int = (int)a_TongueFrontSwitch.Get();

	SmartDashboard::PutNumber("Tongue Front int", tongue_front_int);
	SmartDashboard::PutNumber("Tongue Back int", tongue_back_int);

	TongueState nextState = a_TongueState;
	switch (a_TongueState) {
	case kExtending:

		SmartDashboard::PutString("Tongue State: ", "Extending");

		a_TongueMotor.Set(-0.5);

		if(tongue_back_int == 0) {
			nextState = kRetracting;
		}
		break;

	case kRetracting:

		SmartDashboard::PutString("Tongue State: ", "Retracting");

		a_TongueMotor.Set(0.5);

		if(tongue_front_int == 0) {
			nextState = kTongueIdle;
			// next state may never run, so stop motor here
			a_TongueMotor.Set(0.0);
		}

		break;

	case kTongueIdle:
		SmartDashboard::PutString("Tongue State: ", "Idle");
		a_TongueMotor.Set(0.0);
		break;
	}
	a_TongueState = nextState;
}

void Tongue::TestUpdate(Joystick &stick, Joystick &stick2) {
	double ForwardSpeed = GetSmartDashboardNumber("TongueForward", -1);
	double BackwardSpeed = GetSmartDashboardNumber("TongueBackward", 0.5);

	if(stick.GetRawButton(11) && a_TonguePiston.Get() == DoubleSolenoid::kForward) {
		a_TongueMotor.Set(ForwardSpeed);
	}else if(stick.GetRawButton(12)) {
		a_TongueMotor.Set(BackwardSpeed);
	}else {
		a_TongueMotor.Set(0.0);
	}

	if(!a_TongueFrontSwitch.Get())
	{
		if(stick2.GetRawButton(4)) {
			a_TonguePiston.Set(DoubleSolenoid::kForward);
		}else if(stick2.GetRawButton(5)) {
			a_TonguePiston.Set(DoubleSolenoid::kReverse);
		}
	}
	SmartDashboard::PutBoolean("Tongue Front Switch", a_TongueFrontSwitch.Get());
	SmartDashboard::PutBoolean("Tongue Back Switch", a_TongueBackSwitch.Get());
}

TongueState Tongue::GetState() {
	return a_TongueState;
}

void Tongue::lol() {
	a_TonguePiston.Set(DoubleSolenoid::kForward);
}

void Tongue::SetEnabled(bool valToSet) {
	enabled = valToSet;
}
