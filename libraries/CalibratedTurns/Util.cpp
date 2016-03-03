#include "Util.hpp"

//Normalize to [-180,180):
float constrainAngle(float x){
    x = fmod(x + M_PI,M_2PI);
    if (x < 0)
        x += M_2PI;
    return x - M_PI;
}
// convert to [-360,360]
float angleConv(float angle){
    return fmod(constrainAngle(angle),M_2PI);
}
float angleDiff(float a,float b){
    float dif = fmod(b - a + M_PI,M_2PI);
    if (dif < 0)
        dif += M_2PI;
    return dif - M_PI;
}