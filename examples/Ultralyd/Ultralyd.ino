#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define TRIGGER_PIN 5
#define ECHO_PIN 6
#define MAX_DISTANCE 80

#define QTR_THRESHOLD  300

//Speeds for turning
#define REVERSE_SPEED     200 // 0 is stopped, 400 is full speed
#define TURN_SPEED        200
#define FORWARD_SPEED     100
#define REVERSE_DURATION  200 // ms
#define TURN_DURATION     300 // ms

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int uS;

unsigned long timeSeen;

boolean driveForward = false;
unsigned long spinTime = 0;
unsigned long driveTime = 0;
 
void setup() {
   Serial.begin(9600);

   sensors.init();

   button.waitForButton();
}
 
void loop() {
  
  //Reading IR-Sensors
  sensors.read(sensor_values);

  //Readign Sonar every 10ms
  if (millis() %1 == 0) {
    uS = sonar.ping_cm();
    Serial.println(uS);
  }

  //Figurign out what to do

  if (sensor_values[0] < QTR_THRESHOLD){
    // if leftmost sensor detects line, reverse and turn to the right
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }else if (sensor_values[5] < QTR_THRESHOLD){
    // if rightmost sensor detects line, reverse and turn to the left
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  }else if (uS < 70 && uS > 0){ // Drive straight is it detects something in range 0 - 40.
    motors.setSpeeds(400, 400); //set left and right motor speed
    timeSeen = millis();
  }else if (millis() - timeSeen < 150){
    motors.setSpeeds(-200, 200);
  }else if (millis() - timeSeen < 450){
    motors.setSpeeds(200, -200);
  }else if (driveForward){
    if (driveTime == 0){
      driveTime = millis();
    }
    motors.setSpeeds(300, 300);
    if (millis() - driveTime > 500){
      driveForward = false;
      driveTime = 0;
    }
  }else{
    if (spinTime == 0){
      spinTime = millis();
    }
    motors.setSpeeds(-300, 300); // Spins around if it doesn't see anything.
    if (millis() - spinTime > 800){
      driveForward = true;
      spinTime = 0;
    }
  }
}
