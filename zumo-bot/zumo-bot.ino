#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

// added a symbolic link from zumo-g21\libraries to my arduino\libraries folder. this works.

//Speeds for turning
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

#define NUM_SENSORS 6

#define QTR_THRESHOLD  1500

#define MAX_DISTANCE 70

#define LASTSEEN_INTERVAL 500

unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

volatile unsigned int sonarR_distance;
//unsigned int sonarC_distance;
volatile unsigned int sonarL_distance;

char lastSeen;

int timer = 0;

void setup() {
  Serial.begin(9600);
  sensors.init();
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  button.waitForButton();

  pinMode(3, OUTPUT); //
  pinMode(6, OUTPUT); //
}

void receiveEvent(int bytes) {
	sonarR_distance = Wire.read();
	//sonarC_distance = Wire.read();
	sonarL_distance = Wire.read();
}

void loop() {

  if (lastSeen !='N' && millis()-timer>LASTSEEN_INTERVAL) {
      lastSeen = 'N';
      timer = 0;
  }

  sensors.read(sensor_values);
//  Serial.print(sensor_values[0]);
//  Serial.print(" ");
//  Serial.println(sensor_values[5]);
  Serial.print(sonarR_distance);
  Serial.print(" ");
  //Serial.print(sonarC_distance);
  Serial.print(" ");
  Serial.println(sonarL_distance);
  
  if (sensor_values[0] < QTR_THRESHOLD){
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    lastSeen = 'N';
  } 
  else if (sensor_values[5] < QTR_THRESHOLD) {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    lastSeen = 'N';
  } 
  else {

    motors.setSpeeds(0, 0);

    if (sonarR_distance < MAX_DISTANCE && sonarR_distance > 0){
      digitalWrite(6, HIGH);
    }else{
      digitalWrite(6, LOW);
    }
    if (sonarL_distance < MAX_DISTANCE && sonarL_distance > 0){
      digitalWrite(3, HIGH);
    }else{
      digitalWrite(3, LOW);
    }

    
    if (sonarR_distance < MAX_DISTANCE && sonarR_distance > 0 && sonarL_distance < MAX_DISTANCE && sonarL_distance > 0 && abs(sonarR_distance - sonarL_distance < 5)) {
      motors.setSpeeds(400,400);
      lastSeen = 'N';
    } 
    else if (sonarR_distance < MAX_DISTANCE && sonarR_distance > 0) {
      lastSeen = 'R';
      motors.setSpeeds(400,300);
      timer = millis();
    } 
    else if (sonarL_distance < MAX_DISTANCE && sonarL_distance > 0) {
      lastSeen = 'L';
      motors.setSpeeds(300,400);
      timer = millis();
    } 
    else if (lastSeen == 'R') {
      motors.setSpeeds(400,-100);
    } 
    else if (lastSeen == 'L') {
      motors.setSpeeds(-100,400);
    } 
    else {
      motors.setSpeeds(200, -200); // Spins around if it doesn't see anything.
    }
  }
}
