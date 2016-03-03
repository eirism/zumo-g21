#include "Arduino.h"
#include "Utils.h"

Utils::Utils() {

}

void Utils::resetEcho(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(50);
    pinMode(pin, INPUT);
}