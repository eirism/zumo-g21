#include <PLab_ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <EEPROM.h>
#include <L3G.h>
#include <LSM303.h>

// added a symbolic link from zumo-g21\libraries to my arduino\libraries folder. this works.
#include <CalibratedTurns.h>
//#include "../../libraries/CalibratedTurns/CalibratedTurns.h"
//#include "C:/Users/admin/Dropbox/code/ProgLab/zumo-g21/libraries/CalibratedTurns/CalibratedTurns.hpp"

void setup(){
	CalibratedTurnsSetup();
}

void loop(){
	CalibratedTurnsLoop();
}
