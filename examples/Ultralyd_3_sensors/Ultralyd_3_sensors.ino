#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

// added a symbolic link from zumo-g21\libraries to my arduino\libraries folder. this works.
#include <Utils.h>

#define TRIGGER_PIN_SONAR_R 5
#define TRIGGER_PIN_SONAR_C 6
#define TRIGGER_PIN_SONAR_L 3
#define ECHO_PIN_R 4
#define ECHO_PIN_C 15
#define ECHO_PIN_L 14
#define MAX_DISTANCE 70

#define QTR_THRESHOLD  1500

//Speeds for turning
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

#define NUM_SENSORS 6

#define SONAR_INTERVAL 50

unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
NewPing sonarR(TRIGGER_PIN_SONAR_R, ECHO_PIN_R, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarC(TRIGGER_PIN_SONAR_C, ECHO_PIN_C, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarL(TRIGGER_PIN_SONAR_L, ECHO_PIN_L, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Utils utils;

unsigned int sonarR_distance;
unsigned int sonarC_distance;
unsigned int sonarL_distance;

char lastSeen;

long timer;

void setup() {
  Serial.begin(9600);
  sensors.init();
  button.waitForButton();
  timer=0;
}

void loop() {

  /*
  sensors.read(sensor_values);
  Serial.print(sensor_values[0]);
  Serial.print(" ");
  Serial.println(sensor_values[5]);
  delay(100) ;

  if (sensor_values[0] < QTR_THRESHOLD){
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  } 
  else if (sensor_values[5] < QTR_THRESHOLD) {
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  } 
  else*/ if((millis()-timer)>=SONAR_INTERVAL){  // runs at 50Hz
    timer=millis();

    sonarR_distance = sonarR.ping_cm();
    sonarC_distance = sonarC.ping_cm();
    sonarL_distance = sonarL.ping_cm();

    Serial.print(sonarR_distance);
    Serial.print(" ");
    Serial.print(sonarC_distance);
    Serial.print(" ");
    Serial.println(sonarL_distance);
    if ((sonarR_distance == 0 ) && digitalRead(ECHO_PIN_R) == HIGH) {
      utils.resetEcho(ECHO_PIN_R);   
    }
    if ((sonarL_distance == 0 ) && digitalRead(ECHO_PIN_L) == HIGH) {
      utils.resetEcho(ECHO_PIN_L);   
    } 

    if (sonarC_distance < 70 && sonarC_distance > 0 && sonarC_distance < sonarR_distance && sonarC_distance < sonarL_distance) {
      motors.setSpeeds(400,400);
    } 
    else if (sonarR_distance < 70 && sonarR_distance > 0 && sonarR_distance < sonarC_distance && sonarR_distance < sonarL_distance) {
      lastSeen = 'R';
      motors.setSpeeds(400,300);
    } 
    else if (sonarL_distance < 70 && sonarL_distance > 0 && sonarL_distance < sonarC_distance && sonarL_distance < sonarR_distance) {
      lastSeen = 'L';
      motors.setSpeeds(300,400);
    } 
    else if (lastSeen == 'R') {
      motors.setSpeeds(400,100);
    } 
    else if (lastSeen == 'L') {
      motors.setSpeeds(100,400);
    } 
    else {
      motors.setSpeeds(-300, 300); // Spins around if it doesn't see anything.
    }
  }
}
