#include <PLab_ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <EEPROM.h>
#include <L3G.h>
#include <LSM303.h>

#include <CalibratedTurns.h>

void setup(){
	CalibratedTurnsSetup();
}

void loop(){
	CalibratedTurnsLoop();
}
