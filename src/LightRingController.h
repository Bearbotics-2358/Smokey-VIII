#ifndef LIGHTRINGCONTROLLER_H
#define LIGHTRINGCONTROLLER_H

#include <WPILib.h>

class LightRingController {
public:
	LightRingController();
	bool SetColor(int device, int r, int g, int b);
private:
	I2C a_I2C;
};

#endif // LIGHTRINGCONTROLLER_H
