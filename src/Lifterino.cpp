#include "Lifterino.h"
#include "Prefs.h"



Lifterino::Lifterino()
	:a_Rlifter(RIGHTLIFTER_PORT),
	 a_Llifter(LEFTLIFTER_PORT),
	 a_RGripper(RGRIPPER_EXTEND_SOLENOID, RGRIPPER_RETRACT_SOLENOID),
	 a_LGripper(LGRIPPER_EXTEND_SOLENOID, LGRIPPER_RETRACT_SOLENOID),
	 a_Encoder(LIFT_ENCODER_PORT_1, LIFT_ENCODER_PORT_2),
	 a_State(kNoTotes),
	 a_Timer(){

}

void Lifterino::Update(Joystick &stick) {
	bool liftButton = stick.GetRawButton(1);
	LifterinoState nextState = a_State;

	switch(a_State){
	case kNoTotes:
		if(liftButton){
			nextState = kGrip;
		}
		break;

	case kGrip:
		a_RGripper.Set(DoubleSolenoid::kForward);
		a_LGripper.Set(DoubleSolenoid::kForward);
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
		if(a_Encoder.Get() >= 6){
			nextState = kIdleWithTote;
		}
		break;
	case kIdleWithTote:
		break;
	}


	a_State = nextState;


}
