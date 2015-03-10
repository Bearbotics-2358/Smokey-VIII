#include "Tongue.h"
#include "Prefs.h"
#include "Utilities.h"

Tongue::Tongue()
: a_TonguePiston(TONGUE_EXTEND_PORT, TONGUE_RETRACT_PORT),
  a_TongueMotor(TONGUE_PORT),
  a_TongueFrontSwitch(TONGUE_SWITCH_PORT),
  a_TongueBackSwitch(TONGUE_BACK_SWITCH_PORT),
  a_TongueState(kTongueIdle)
{

}

void Tongue::Update(Joystick &stick, Joystick &stick2) {

}

void Tongue::Raise() {
	a_TonguePiston.Set(DoubleSolenoid::kForward);
}

void Tongue::Extend(bool startingLoop)
{
	int tongue_back_int = 0;
	if(startingLoop)
		{
			a_TongueState = kExtending;
		}
	TongueState nextState = a_TongueState;
	switch (a_TongueState) {
	case kExtending:
		tongue_back_int = (int)a_TongueBackSwitch.Get();
		SmartDashboard::PutBoolean("Tongue Back Switch", a_TongueBackSwitch.Get());
		SmartDashboard::PutString("Tongue State: ", "Extending");
		SmartDashboard::PutNumber("Tongue Back Switch int", tongue_back_int );
		startingLoop = false;
		if(tongue_back_int) {
			a_TongueMotor.Set(-0.5);
		} else {
			nextState = kRetracting;
		}
		break;

	case kRetracting:
		SmartDashboard::PutBoolean("Tongue Back Switch", a_TongueBackSwitch.Get());
		SmartDashboard::PutString("Tongue State: ", "Retracting");
		if(a_TongueFrontSwitch.Get() == true)
		{
			a_TongueMotor.Set(0.5);
		}
		else
		{
			nextState = kTongueIdle;
		}
		break;

	case kTongueIdle:
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
