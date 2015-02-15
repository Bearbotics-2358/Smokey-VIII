#include "Tongue.h"
#include "Prefs.h"

Tongue::Tongue()
: a_TonguePiston(TONGUE_EXTEND_PORT, TONGUE_RETRACT_PORT),
  a_TongueMotor(TONGUE_PORT)
{

}

void Tongue::Update(Joystick &stick, Joystick &stick2) {

}

void Tongue::TestUpdate(Joystick &stick, Joystick &stick2) {

}
