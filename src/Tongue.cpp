#include "Tongue.h"
#include "Prefs.h"

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
	if(startingLoop)
		{
			a_TongueState = kExtending;
		}
	switch (a_TongueState) {
	case kExtending:
		startingLoop = false;
		if(a_TongueBackSwitch.Get())
		{
			a_TongueMotor.Set(.5);
		}
		else
		{
			a_TongueState = kRetracting;
		}
		break;

	case kRetracting:
		if(a_TongueFrontSwitch.Get())
		{
			a_TongueMotor.Set(-.5);
		}
		else
		{
			a_TongueState = kTongueIdle;
		}
		break;

	case kTongueIdle:
		break;
	}
}

void Tongue::TestUpdate(Joystick &stick, Joystick &stick2) {

	if(stick.GetRawButton(11)) {
		a_TongueMotor.Set(-1.0);
	}else if(stick.GetRawButton(12)) {
		a_TongueMotor.Set(1.0);
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
	SmartDashboard::PutBoolean("Tongue Switch", a_TongueFrontSwitch.Get());
}
