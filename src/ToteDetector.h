#ifndef TOTE_DETECTOR_H
#define TOTE_DETECTOR_H

#include "WPILib.h"

class ToteDetector {
public:
	ToteDetector(std::string ip);

	void SnapImage();
	bool CheckForTote(bool snapImage);

private:
	void CheckIMAQError(int rval, std::string desc);

	AxisCamera a_Camera;
	DigitalOutput a_Leds;

	HSLImage a_NoFlashImage;
	HSLImage a_FlashImage;
};

#endif // TOTE_DETECTOR_H
