#include "Arduino.h"
#include "Utils.h"

Utils::Utils(int delay, int IRThreshold) {
    _delay = delay;
    _IRThreshold = IRThreshold;
}

void Utils::resetEcho(int pin) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(_delay);
    pinMode(pin, INPUT);
}

bool Utils::checkIRFront(int pin) {
    float reading = analogRead(pin);
    return (reading < _IRThreshold);
}
