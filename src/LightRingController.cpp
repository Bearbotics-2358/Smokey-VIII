#include <LightRingController.h>
#include "Prefs.h"

LightRingController::LightRingController()
	:a_I2C(I2C::kMXP, LIGHT_RING_CONTROLLER_ADDRESS){


}

bool LightRingController::SetColor(int device, int r, int g, int b){
	uint8_t out[3];
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
	return a_I2C.Transaction(out, 3, NULL, 0);
}
