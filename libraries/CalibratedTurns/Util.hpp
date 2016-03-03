#ifndef UTIL
#define UTIL

#include <math.h>
#define M_2PI 2*M_PI

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

float constrainAngle(float x);

float angleConv(float angle);

float angleDiff(float a,float b);
#endif