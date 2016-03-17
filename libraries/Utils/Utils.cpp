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

void Utils::resetEcho(int pin1, int pin2) {
    pinMode(pin1, OUTPUT);
    pinMode(pin2, OUTPUT);
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    delay(_delay);
    pinMode(pin1, INPUT);
    pinMode(pin2, INPUT);
}

bool Utils::checkIRFront(int pin) {
    float reading = analogRead(pin);
    return (reading < _IRThreshold);
}

