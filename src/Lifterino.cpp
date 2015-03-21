#include <Prefs.h>
#include "Lifterino.h"
#include "PIDController.h"
#include "Utilities.h"

Lifterino::Lifterino()
:a_Rlifter(RIGHTLIFTER_PORT),
 a_Llifter(LEFTLIFTER_PORT),
 a_Grip(GRIPPER_EXTEND_SOLENOID, GRIPPER_RETRACT_SOLENOID),
 a_Encoder(LIFT_ENCODER_PORT_1, LIFT_ENCODER_PORT_2, true, Encoder::k4X),
 a_State(kNoTotes),
 a_AutoState(kFindZero),
 a_Timer(),
 a_LifterSwitch(LIFTER_SWITCH_PORT),
 a_LifterC(a_Llifter, a_Rlifter),
 a_PID(P, I, D, &a_Encoder, &a_LifterC, 0.05)
{
	SetEnabled(false);
	a_Encoder.SetPIDSourceParameter(PIDSource::kDistance);
	a_Encoder.SetDistancePerPulse(1);
	a_PID.SetOutputRange(-.5, 1.0);
	a_PID.SetAbsoluteTolerance(2.9);
	a_PID.SetInputRange(0, 70);
	a_PID.SetPID(P, I, D);
	enabled = true;
	a_Rlifter.SetSafetyEnabled(false);
	a_Llifter.SetSafetyEnabled(false);
}

void Lifterino::Update(Joystick &stick, Joystick &stick2) {
	if(stick2.GetRawButton(6)) {
		SwitchToManual(false);
		a_PID.Disable();
	} else if(stick2.GetRawButton(7)) {
		SwitchToManual(true);
		a_PID.Enable();
	}

	// must make sure you set motors every periodic cycle
	MotorSafeFeed();

	if(enabled) {
		bool liftButton = stick.GetRawButton(1);

		LifterinoState nextState = a_State;

		SmartDashboard::PutNumber("Encoder Value", a_Encoder.GetDistance());
		SmartDashboard::PutBoolean("Lifter Switch", a_LifterSwitch.Get());
		SmartDashboard::PutNumber("Lifter Speed", a_PID.Get());
		SmartDashboard::PutNumber("PID Error", a_PID.GetError());
		SmartDashboard::PutNumber("Lifter State", a_State);

		if(!a_LifterSwitch.Get()) {
			a_Encoder.Reset();
		}

		// State Machine
		switch(a_State){
		case kFindZero:
			a_LifterC.Set(-0.1);
			if(!a_LifterSwitch.Get()) {
				a_LifterC.Set(0.0);
				a_PID.Enable();
				a_PID.SetSetpoint(0.0);
				nextState = kNoTotes;

			}
			break;

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
			a_PID.SetSetpoint(TOP_LIFTER_SETPOINT);
			if(a_PID.OnTarget()) {
				nextState = kIdleWithTote;
			}
			break;

		case kIdleWithTote:
			if(liftButton){
				nextState = kMoveDown;
			}else if(stick.GetRawButton(10)){
				nextState = kPlacing;
			}
			break;

		case kMoveDown:
			a_PID.SetSetpoint(MOVE_DOWN_SETPOINT);
			if(a_PID.OnTarget()) {
				nextState = kRelease;
			}
			break;

		case kLower:
			a_PID.SetSetpoint(BOTTOM_LIFTER_SETPOINT);
			if(a_PID.OnTarget()){
				nextState = kGrip;
			}
			break;

		case kReleaseDelay:
			if(a_Timer.Get() >= 1){
				nextState = kLower;
			}
			break;

		case kPlacing:
			a_PID.SetSetpoint(BOTTOM_LIFTER_SETPOINT);
			if(a_PID.OnTarget()) {
				a_Grip.Set(DoubleSolenoid::kReverse);
				nextState = kNoTotes;
			}
			break;
		}

		a_State = nextState;

	} else {
		SmartDashboard::PutNumber("Encoder Value", a_Encoder.GetDistance());
		SmartDashboard::PutBoolean("Lifter Switch", a_LifterSwitch.Get());
		SmartDashboard::PutNumber("Lifter Speed", a_PID.Get());
		SmartDashboard::PutNumber("PID Error", a_PID.GetError());
		SmartDashboard::PutNumber("Lifter State", a_State);


		bool GripExtendButton = stick2.GetRawButton(3);
		bool GripRetractButton = stick2.GetRawButton(2);

		if(!a_LifterSwitch.Get()){
			a_Encoder.Reset();
		}

		double soke = GetSmartDashboardNumber("soke", 60);

		if(stick2.GetRawButton(8)) {
			SetEnabled(false);
		}

		if(stick2.GetRawButton(1)){
			SetEnabled(true);
			a_PID.SetSetpoint(soke);
		}
		if(stick2.GetRawButton(9)){
			SetEnabled(true);
			a_PID.SetSetpoint(1);
		}else if(stick2.GetRawButton(10)){
			SetEnabled(true);
			a_PID.SetSetpoint(30);
		}else if(stick2.GetRawButton(11)){
			SetEnabled(true);
			a_PID.SetSetpoint(40);
		}


		if(!a_enabled) {
			if(a_LifterSwitch.Get() || (stick2.GetY() <= 0)){
				a_LifterC.Set(-1 * stick2.GetY());
			}
		}

		if(GripExtendButton) {
			a_Grip.Set(DoubleSolenoid::kForward);
		} else if(GripRetractButton) {
			a_Grip.Set(DoubleSolenoid::kReverse);
		}
	}
}

void Lifterino::AutonUpdate(void) {
	LifterinoState nextState = a_AutoState;
	/* if(a_AutoState == kIdleWithTote)
	{
		nextState = kRelease;
	} */

	// must make sure you set motors every periodic cycle
	MotorSafeFeed();

	if(!a_LifterSwitch.Get()) {
		a_Encoder.Reset();
	}

	// State Machine
	switch(a_AutoState){
	case kFindZero:
		a_LifterC.Set(-0.1);
		if(!a_LifterSwitch.Get()) {
			nextState = kNoTotes;
			a_PID.Enable();
		}
		break;

	case kNoTotes:
		nextState = kGrip;
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
		if(a_Timer.Get() >= 0.5 ){
			nextState = kLift;
		}
		break;

	case kLift:
		a_PID.SetSetpoint(TOP_LIFTER_SETPOINT);
		if(a_PID.OnTarget()) {
			nextState = kIdleWithTote;
		}
		break;

	case kIdleWithTote:

		break;

	case kLower:
		a_PID.SetSetpoint(BOTTOM_LIFTER_SETPOINT);
		if(a_PID.OnTarget()){
			nextState = kGrip;
		}
		break;

	case kReleaseDelay:
		if(a_Timer.Get() >= 0.5){
			nextState = kLower;
		}
		break;

	case kMoveDown:
		a_PID.SetSetpoint(MOVE_DOWN_SETPOINT);
		if(a_PID.OnTarget()) {
			nextState = kRelease;
		}
		break;

	case kPlacing:

		break;

	}

	a_AutoState = nextState;
}

void Lifterino::TestUpdate(Joystick &stick, Joystick &stick2) {
	// must make sure you set motors every periodic cycle
	MotorSafeFeed();

	SmartDashboard::PutNumber("P", P);
	SmartDashboard::PutNumber("I", I);
	SmartDashboard::PutNumber("D", D);

	SmartDashboard::PutNumber("Encoder Value", a_Encoder.GetDistance());
	SmartDashboard::PutBoolean("Lifter Switch", a_LifterSwitch.Get());
	SmartDashboard::PutNumber("Lifter Speed", a_PID.Get());
	SmartDashboard::PutNumber("PID Error", a_PID.GetError());
	SmartDashboard::PutNumber("KiwiSpeed", a_LifterC.Get());
	SmartDashboard::PutNumber("Lifter State", a_State);

	P = GetSmartDashboardNumber("PQ", P);
	I = GetSmartDashboardNumber("IQ", I);
	D = GetSmartDashboardNumber("DQ", D);

	bool GripExtendButton = stick2.GetRawButton(3);
	bool GripRetractButton = stick2.GetRawButton(2);

	if(!a_LifterSwitch.Get()){
		a_Encoder.Reset();
	}

	double soke = GetSmartDashboardNumber("soke", 40);

	if(stick2.GetRawButton(1)){
		SetEnabled(true);
		a_PID.SetSetpoint(soke);
	}
	if(stick2.GetRawButton(9)){
		SetEnabled(true);
		a_PID.SetSetpoint(1);
	}else if(stick2.GetRawButton(10)){
		SetEnabled(true);
		a_PID.SetSetpoint(30);
	}else if(stick2.GetRawButton(11)){
		SetEnabled(true);
		a_PID.SetSetpoint(40);
	}

	if(stick2.GetRawButton(6)){
		a_PID.SetPID(P, I, D);
	}

	if(stick2.GetRawButton(8)) {
		SetEnabled(false);
	}

	if(!a_enabled) {
		if(a_LifterSwitch.Get() || (stick2.GetY() <= 0)){
			a_LifterC.Set(-1 * stick2.GetY());
		}
	}

	if(GripExtendButton) {
		a_Grip.Set(DoubleSolenoid::kForward);
	} else if(GripRetractButton) {
		a_Grip.Set(DoubleSolenoid::kReverse);
	}
}

void Lifterino::MotorSafeFeed(void)
{
	// must make sure you set motors every periodic cycle
	if(a_enabled) {
		// if a_enabled, PID is running and takes care of it
	} else {
		// PID not running
		double val = a_LifterC.Get();
		a_LifterC.Set(val);
	}
}

void Lifterino::Reset(void) {
	a_Grip.Set(DoubleSolenoid::kReverse);
	a_PID.SetSetpoint(0.0);
	a_State = kFindZero;

}

void Lifterino::SetEnabled(bool enable)
{
	a_enabled = enable;
	(enable) ? a_PID.Enable() : a_PID.Disable();
}

LifterinoState Lifterino::GetAutoState()
{
	return a_AutoState;
}

void Lifterino::SetState(LifterinoState stateToSet)
{
	a_AutoState = stateToSet;
}

void Lifterino::SwitchToManual(bool valToSet)
{
	enabled = valToSet;
}
