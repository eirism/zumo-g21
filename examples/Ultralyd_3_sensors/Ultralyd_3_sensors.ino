#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <NewPing.h>

#define TRIGGER_PIN_SONAR_R 1
#define TRIGGER_PIN_SONAR_C 2
#define TRIGGER_PIN_SONAR_L 3
#define ECHO_PIN 4
#define MAX_DISTANCE 80

#define QTR_THRESHOLD  1500

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
NewPing sonarR(TRIGGER_PIN_SONAR_R, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarC(TRIGGER_PIN_SONAR_C, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarL(TRIGGER_PIN_SONAR_L, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sonarR_distance;
unsigned int sonarC_distance;
unsigned int sonarL_distance;


void setup() {
  Serial.begin(9600);
  sensors.init();
  button.waitForButton();
}

void loop() {
  //Reading IR-Sensors
  sensors.read(sensor_values);
  Serial.print(sensor_values[0]);
  Serial.print(", ");
  Serial.println(sensor_values[5]);

  if (millis() %1 == 0) {
    sonarR_distance = sonarR.ping_cm();
    sonarC_distance = sonarC.ping_cm();
    sonarL_distance = sonarL.ping_cm();
  }

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
  }else if (sonarC_distance < 70 && sonarC_distance > 0) {
    motors.setSpeeds(400,400);
  }else if (sonarR_distance < 70 && sonarR_distance > 0) {
    motors.setSpeeds(400,200);
  }else if (sonarL_distance < 70 && sonarL_distance > 0) {
    motors.setSpeeds(200,400);
  }else{
    motors.setSpeeds(-300, 300); // Spins around if it doesn't see anything.
  }
}
