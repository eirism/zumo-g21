#ifndef TEST_HPP
#define TEST_HPP

#include <Wire.h>
#include <EEPROM.h>
#include <math.h>
#include "Util.hpp"
#include <L3G.h>
#include <LSM303.h>


#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer 

#define Gyro_Gain_X 0.07 //X axis Gyro gain
#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
#define Gyro_Gain_Z 0.07 //Z axis Gyro gain
#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z)) //Return the scaled ADC raw data of the gyro in radians for second

#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002

#define OUTPUTMODE 1

//#define PRINT_DCM 0     //Will print the whole direction cosine matrix
#define PRINT_ANALOGS 0 //Will print the analog raw data
#define PRINT_EULER 1   //Will print the Euler angles Roll, Pitch and Yaw


void minIMU_setup();
void minIMU_loop();

void Compass_Heading();

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
void Normalize(void);
void Drift_correction(void);
void Matrix_update(void);
void Euler_angles(void);

float Vector_Dot_Product(float vector1[3],float vector2[3]);
void Vector_Cross_Product(float vectorOut[3], float v1[3],float v2[3]);
void Vector_Scale(float vectorOut[3],float vectorIn[3], float scale2);
void Vector_Add(float vectorOut[3],float vectorIn1[3], float vectorIn2[3]);

void Matrix_Multiply(float a[3][3], float b[3][3],float mat[3][3]);

long convert_to_dec(float x);

void I2C_Init();
void Gyro_Init();
void Read_Gyro();
void Accel_Init();
void Read_Accel();
void Compass_Init();
void Read_Compass();


#endif
