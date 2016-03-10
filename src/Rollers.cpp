#include <Prefs.h>

#include "Rollers.h"


Rollers::Rollers()
:a_LRoller(LEFT_ROLLER_PORT),
 a_RRoller(RIGHT_ROLLER_PORT),
 a_RollerSolenoid(ROLLER_EXTEND, ROLLER_RETRACT)
{

}

void Rollers::Update(Joystick &stick, Joystick &stick2) {
	if(stick2.GetRawButton(4)) {
		a_RollerSolenoid.Set(DoubleSolenoid::kForward);
	}else if(stick2.GetRawButton(5)) {
		a_RollerSolenoid.Set(DoubleSolenoid::kReverse);
	}

	if(stick.GetRawButton(11)) {
		a_RRoller.Set(-0.4);
		a_LRoller.Set(0.4);
	}else if(stick.GetRawButton(12)) {
		a_RRoller.Set(0.4);
		a_LRoller.Set(-0.4);
	}else {
		a_RRoller.Set(0.0);
		a_LRoller.Set(0.0);
	}

}

void Rollers::TestUpdate(Joystick &stick, Joystick &stick2) {
	if(stick2.GetRawButton(4)) {
		a_RollerSolenoid.Set(DoubleSolenoid::kForward);
	}else if(stick2.GetRawButton(5)) {
		a_RollerSolenoid.Set(DoubleSolenoid::kReverse);
	}

	if(stick.GetRawButton(11)) {
		a_RRoller.Set(-0.4);
		a_LRoller.Set(0.4);
	}else if(stick.GetRawButton(12)) {
		a_RRoller.Set(0.4);
		a_LRoller.Set(-0.4);
	}else {
		a_RRoller.Set(0.0);
		a_LRoller.Set(0.0);
	}

}
