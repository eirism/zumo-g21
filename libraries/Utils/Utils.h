//
// Created by eirik on 3/3/16.
//

#ifndef Utils_h
#define Utils_h

#include "Arduino.h"

class Utils {
    public:
        Utils(int delay, int IRThreshold);
        void resetEcho(int pin);
        void resetEcho(int pin1, int pin2);
        bool checkIRFront(int pin);
    private:
        int _delay;
        int _IRThreshold;
};

#endif
