#ifndef SMOKEY_VIII_H
#define SMOKEY_VIII_H

#include "WPIlib.h"

class Smokey_VIII : public IterativeRobot{
public:
	Smokey_VIII(void);
	~Smokey_VIII(void);
	void RobotInit(void);
	void TeleopInit(void);
	void TeleopPeriodic(void);
	void TestPeriodic(void);

private:
	Gyro a_Gyro;
	Joystick a_Joystick;
	CANTalon a_FLmotor;
	CANTalon a_FRmotor;
	CANTalon a_BLmotor;
	Talon a_Tongue;
	Talon a_Rlifter;
	Talon a_Llifter;
};

#endif // SMOKEY_VIII_H
