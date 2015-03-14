#include "ToteDetector.h"
#include "Prefs.h"

#include <networktables2/util/System.h>
#include <nivision.h>
#include <stdexcept>
#include <sstream>
#include <time.h>
#include <vector>

const static int IMAGE_THRESHOLD = 50;
const static int CAMERA_DELAY    = 100;
const static int MINIMUM_SCORE   = 800;
const static int MINIMUM_SIZE    = 700;

  ToteDetector::ToteDetector(std::string ip)
  : a_Camera(ip),
    a_Flash()
{
  a_Flash.SetFlash(0,false);
}

void ToteDetector::CheckIMAQError(int rval, std::string desc)
{
  if(rval == 0)
  {
    int error = imaqGetLastError();

    std::ostringstream errorDesc;
    errorDesc << error << " " << desc;

    throw std::runtime_error(errorDesc.str());
  }
}

void ToteDetector::SnapImage() {
  if(!a_Camera.IsFreshImage()) {
    throw std::runtime_error("No fresh image");
  }

  timespec start, end;

  // Turn the flash on
  a_Flash.SetFlash(0, true);

  // Snap no-flash image
  a_Camera.GetImage(&a_NoFlashImage);

  clock_gettime(CLOCK_MONOTONIC, &start);

  // Sleep for a bit
  sleep_ms(CAMERA_DELAY);

  // Wait for new image
  while(!a_Camera.IsFreshImage());

  // Snap flash image
  a_Camera.GetImage(&a_FlashImage);

  clock_gettime(CLOCK_MONOTONIC, &end);

  // Turn flash back off
  a_Flash.SetFlash(0,false);

  CheckIMAQError(imaqWriteJPEGFile(a_NoFlashImage.GetImaqImage(),
        "/home/lvuser/00-noflash.jpg", 1000, NULL),
      "imaqWriteJPEGFile");
  CheckIMAQError(imaqWriteJPEGFile(a_FlashImage.GetImaqImage(),
        "/home/lvuser/01-flash.jpg", 1000, NULL),
      "imaqWriteJPEGFile");

  SmartDashboard::PutNumber("Frame Delta (ms)",
      (end.tv_nsec - start.tv_nsec) / 1000000);
}

bool ToteDetector::CheckForTote(bool snapImage) {
  int rval;
  int width1, width2, height1, height2;
  timespec start, end;

  if(snapImage) {
    SnapImage();
  }

  clock_gettime(CLOCK_MONOTONIC, &start);

  MonoImage *noFlashMono = a_NoFlashImage.GetLuminancePlane();
  Image *noFlash = noFlashMono->GetImaqImage();

  MonoImage *flashMono = a_FlashImage.GetLuminancePlane();
  Image *flash = flashMono->GetImaqImage();

  rval = imaqGetImageSize(noFlash, &width1, &height1);
  CheckIMAQError(rval, "imaqGetImageSize");

  rval = imaqGetImageSize(noFlash, &width2, &height2);
  CheckIMAQError(rval, "imaqGetImageSize");

  rval = imaqSubtract(noFlash, flash, noFlash);
  CheckIMAQError(rval, "imaqSubtract");

  CheckIMAQError(imaqWriteJPEGFile(noFlash,
        "/home/lvuser/02-subtract.jpg", 1000, NULL),
      "imaqWriteJPEGFile");

  rval = imaqThreshold(noFlash, noFlash, IMAGE_THRESHOLD, 255, 1, 255);
  CheckIMAQError(rval, "imaqThreshold");

  CheckIMAQError(imaqWriteJPEGFile(noFlash,
        "/home/lvuser/03-threshold.jpg", 1000, NULL),
      "imaqWriteJPEGFile");

  // Creates a temporary image that will be used to perform the search.
  Image *shapeImage = imaqCreateImage(IMAQ_IMAGE_U8, 7);

  // Applies a lookup table to the image because the input image for the
  // imaqMatchShape function must be a binary image that contains only
  // pixel values of 0 or 1
  short lookupTable[256];
  lookupTable[0] = 0;
  for(int i = 1 ; i < 256 ; i++) {
    lookupTable[i] = 1;
  }

  rval = imaqLookup(shapeImage, noFlash, lookupTable, NULL);
  CheckIMAQError(rval, "imaqLookup");

  // Creates and read the image template containing the shape to match
  Image *imageTemplate = imaqCreateImage(IMAQ_IMAGE_U8, 7);
  rval = imaqReadFile(imageTemplate, "/home/lvuser/template.png", NULL, NULL);
  CheckIMAQError(rval, "imaqReadFile");

  // Applies the same lookup table to obtain an image containing only pixel
  // values of 0 and 1.
  rval = imaqLookup(imageTemplate, imageTemplate, lookupTable, NULL);
  CheckIMAQError(rval, "imaqLookup");

  // Finds the shape in the binary image.
  int numMatchesFound;
  ShapeReport* shapeReport = imaqMatchShape(shapeImage, shapeImage,
      imageTemplate, 1, 1, 0.5, &numMatchesFound);

  clock_gettime(CLOCK_MONOTONIC, &end);

  SmartDashboard::PutNumber("Image Processing Time (ms)",
      (end.tv_nsec - start.tv_nsec) / 1000000);

  std::vector<ShapeReport> goodMatches;
  printf("==== Shape Match Results ====\n");
  for(int i = 0; i < numMatchesFound; i++) {
    if(shapeReport[i].score >= MINIMUM_SCORE &&
       shapeReport[i].size >= MINIMUM_SIZE) {
      printf("# Shape Match\n");
      printf("- size: %d\n", shapeReport[i].size);
      printf("- x: %d\n- y: %d\n", shapeReport[i].coordinates.left,
          shapeReport[i].coordinates.top);
      printf("- w: %d\n- h: %d\n", shapeReport[i].coordinates.width,
          shapeReport[i].coordinates.height);
      goodMatches.push_back(shapeReport[i]);
    }
  }

  imaqDispose(shapeImage);
  imaqDispose(shapeReport);

  // At this point, goodMatches contains the data about
  // all the possible matches that passed the initial
  // filter criteria. If our lighting and positioning
  // are good enough, goodMatches may always contain
  // the correct two matches (corresponding to the two
  // vision targets on the tote); further testing is
  // needed, i.e. run this code a lot and see what
  // happens.
  //
  // Code to determine the position of the tote might
  // look like this:
  double sumX = 0.0;
  double sumY = 0.0;
  for(auto iter = goodMatches.begin(); iter != goodMatches.end(); ++iter) {
    ShapeReport match = *iter;
    sumX += match.coordinates.left;
    sumY += match.coordinates.top;
  }

  // Average the positions of the matches to determine
  // the center.
  double toteX = (sumX / goodMatches.size());
  double toteY = (sumY / goodMatches.size());

  // TODO: Make this method return the position somehow,
  // or create a new method that does.
  return (goodMatches.size() >= 2);
}
