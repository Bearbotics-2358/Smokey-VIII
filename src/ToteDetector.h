#ifndef TOTE_DETECTOR_H
#define TOTE_DETECTOR_H

#include "WPILib.h"
#include "LightRingController.h"

class ToteDetector {
public:
  typedef struct {
    bool present;
    double x;
    double y;
  } Tote;

  ToteDetector(std::string ip);
  ~ToteDetector();

  void SetDebugMode(bool debugMode);
  bool GetDebugMode();

  void SnapImage();
  Tote FindTote(bool snapImage);

private:
  void CheckIMAQError(int rval, std::string desc);

  bool a_DebugMode;

  AxisCamera a_Camera;
  LightRingController a_Flash;

  HSLImage a_NoFlashImage;
  HSLImage a_FlashImage;
};

#endif // TOTE_DETECTOR_H
