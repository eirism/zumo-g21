#ifndef CALIBRATED_TURNS
#define CALIBRATED_TURNS
#include <PLab_ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <EEPROM.h>
#include <L3G.h>
#include <LSM303.h>
#include <math.h>

#include "MinIMU.h"

void CalibratedTurnsSetup();

void CalibratedTurnsLoop();

#endif