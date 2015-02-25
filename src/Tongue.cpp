#include "Tongue.h"
#include "Prefs.h"

Tongue::Tongue()
: a_TonguePiston(TONGUE_EXTEND_PORT, TONGUE_RETRACT_PORT),
  a_TongueMotor(TONGUE_PORT),
  a_TongueFrontSwitch(TONGUE_SWITCH_PORT)
{

}

void Tongue::Update(Joystick &stick, Joystick &stick2) {

}

void Tongue::Extend()
{

}


void Tongue::TestUpdate(Joystick &stick, Joystick &stick2) {
	if(a_TongueFrontSwitch.Get())
	{
		if(stick.GetRawButton(11)){
			a_TongueMotor.Set(-1.0);
		}
	}
	if(stick.GetRawButton(12)){
		a_TongueMotor.Set(1.0);
	}
	else{
		a_TongueMotor.Set(0.0);
	}

	// a_TongueMotor.Set(stick2.GetX());
	if(!a_TongueFrontSwitch.Get())
	{
		if(stick2.GetRawButton(4)){
			a_TonguePiston.Set(DoubleSolenoid::kForward);
		}else if(stick2.GetRawButton(5)){
			a_TonguePiston.Set(DoubleSolenoid::kReverse);
		}
	}
	SmartDashboard::PutBoolean("Tongue Switch", a_TongueFrontSwitch.Get());
}
