#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

//Speeds for qtr movement
#define REVERSE_SPEED_QTR     300 
#define TURN_SPEED_QTR        300
#define FORWARD_SPEED_QTR     100
#define REVERSE_DURATION_QTR  200 // ms
#define TURN_DURATION_QTR     500 // ms

//Speeds for other movement
#define SEEN_BOTH 400
#define SEEN_RIGHT_L 400
#define SEEN_RIGHT_R 200
#define SEEN_LEFT_L SEEN_RIGHT_R
#define SEEN_LEFT_R SEEN_RIGHT_L
#define LASTSEEN_RIGHT_L 400
#define LASTSEEN_RIGHT_R -100
#define LASTSEEN_LEFT_L LASTSEEN_RIGHT_R
#define LASTSEEN_LEFT_R LASTSEEN_RIGHT_L
#define FIRST_TIME_ROTATE 300
#define SEARCH_ROTATE 300
#define SEARCH_FORWARD 400

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

int timer;
bool seen;
bool firstTime;

int rotateStartTime;

void setup() {
    Serial.begin(9600);
    sensors.init();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    button.waitForButton();
    seen=false;
    pinMode(6, OUTPUT); // indicator for firstTime
    firstTime = true;
    timer = 0;
    rotateStartTime = millis();
}

void receiveEvent(int bytes) {
    sonarR_distance = Wire.read();
    //sonarC_distance = Wire.read();
    sonarL_distance = Wire.read();
}

void loop() {

    if (lastSeen !='N' && (millis()-timer>LASTSEEN_INTERVAL)) {
        lastSeen = 'N';
        timer = 0;
    }

    sensors.read(sensor_values);

    int left_qtr;
    int right_qtr;
    if(!seen){
        left_qtr = 0;
        right_qtr = 5; 
    }
    else if(seen){
        left_qtr = 2;
        right_qtr = 3;
    }

    if (sensor_values[left_qtr] < QTR_THRESHOLD){
        avoid_edge(1);
    } 
    else if (sensor_values[right_qtr] < QTR_THRESHOLD) {
        avoid_edge(-1);
    } 
    else{
        search();
    }
}
void avoid_edge(int dir){
    motors.setSpeeds(-REVERSE_SPEED_QTR, -REVERSE_SPEED_QTR);
    delay(REVERSE_DURATION_QTR);
    motors.setSpeeds(dir * TURN_SPEED_QTR, -dir * TURN_SPEED_QTR);
    delay(TURN_DURATION_QTR);
    motors.setSpeeds(FORWARD_SPEED_QTR, FORWARD_SPEED_QTR);
    lastSeen = 'N';
}
        
void search(){
    motors.setSpeeds(0, 0);

    bool see_right = (sonarR_distance < MAX_DISTANCE && sonarR_distance > 0);
    bool see_left = (sonarL_distance < MAX_DISTANCE && sonarL_distance > 0);
    seen = see_right || see_left;

    firstTime = firstTime && !seen;
    digitalWrite(6, firstTime);

    if (see_right && see_left && abs(sonarR_distance - sonarL_distance) < 5) {
        lastSeen = 'N';
        motors.setSpeeds(SEEN_BOTH,SEEN_BOTH);
    } 
    else if (see_right) {
        lastSeen = 'R';
        motors.setSpeeds(SEEN_RIGHT_L,SEEN_RIGHT_R);
        timer = millis();
    } 
    else if (see_left) {
        lastSeen = 'L';
        motors.setSpeeds(SEEN_LEFT_L,SEEN_LEFT_R);
        timer = millis();
    } 
    else if (lastSeen == 'R') {
        motors.setSpeeds(LASTSEEN_RIGHT_L,LASTSEEN_RIGHT_R);
    } 
    else if (lastSeen == 'L') {
        motors.setSpeeds(LASTSEEN_LEFT_L,LASTSEEN_LEFT_R);
    }
    else if (firstTime) {
        motors.setSpeeds(FIRST_TIME_ROTATE, -FIRST_TIME_ROTATE);
    }
    else if (millis() % 500 < 200){
        motors.setSpeeds(SEARCH_ROTATE, -SEARCH_ROTATE);
    }
    else {
        motors.setSpeeds(SEARCH_FORWARD, SEARCH_FORWARD); 
    }
}

