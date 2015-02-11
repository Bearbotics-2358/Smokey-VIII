#pragma once


#include "SpeedController.h"
#include "PIDOutput.h"
#include "PWM.h"
#include "Talon.h"

class KiwiController :  public SpeedController
{
public:
	KiwiController(Talon &Left, Talon &Right);
	virtual ~KiwiController();
	virtual void Set(float value, uint8_t syncGroup = 0);
	virtual float Get();
	virtual void Disable();

	virtual void PIDWrite(float output);

private:
	Talon &LeftC;
	Talon &RightC;

	float m_speed;
};
