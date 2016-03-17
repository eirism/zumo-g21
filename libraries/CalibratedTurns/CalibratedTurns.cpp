#include "CalibratedTurns.h"
#include "MinIMU.h"

long timer = 0;
float yaw;
int counter=5;

Pushbutton button(ZUMO_BUTTON); 
PLab_ZumoMotors motors;

float setpoint = ToRad(90);

const float kp = 400;
const float ti = 5;
const float td = 0.1;

const int include_ti = 1;
const int include_td = 1;

float old_error = 0;
float cum_error = 0;

const float errorLimit = 0.5 * M_PI/50;
const int speedLimit = 50;

bool isRotating = true;
bool isMoving = false;
int moveCounter = 0;

bool makeSquare = true;

void CalibratedTurnsSetup()
{
	minIMU_setup();
}

void CalibratedTurnsLoop()
{
	if((millis()-timer)>=21){ // 50 Hz loop
		minIMU_loop();
		if(isMoving){
			moveCounter += 1;
			if(moveCounter == 50){
				moveCounter = 0;
				motors.setSpeeds(0, 0);
				isMoving = false;
				setpoint = angleConv(setpoint - ToRad(90));
				isRotating = true;
			}
		}
		else if(isRotating && counter == 0){ // This runs at 10Hz, as often as we get new compass values.
			float error = angleDiff(yaw, setpoint);
			cum_error += error;
			float rotateSpeed = kp * (error +  include_ti*cum_error/ti + include_td * td * (old_error - error));
			motors.setSpeeds(-rotateSpeed, rotateSpeed);
			Serial.print("error: ");
			Serial.println(error);
			Serial.print("rotateSpeed: ");
			Serial.println(rotateSpeed);
			old_error = error;
			if(abs(error)<errorLimit || (false && abs(rotateSpeed)<speedLimit)){
				motors.setSpeeds(0, 0);
				old_error = 0;
				cum_error = 0;
				isRotating = false;
			}
		}

		if(makeSquare && !isRotating){
			isMoving = true;
			motors.setSpeeds(200, 200);
		}
	}
}
