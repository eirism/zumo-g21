#include <Wire.h>
#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>

#define GREEN_LED 6

//Speeds for qtr movement
#define REVERSE_SPEED_QTR     300 
#define TURN_SPEED_QTR        300
#define FORWARD_SPEED_QTR     100
#define REVERSE_DURATION_QTR  200 // ms
#define TURN_DURATION_QTR     500 // ms

//Speeds for other movement
#define SEEN_BOTH 400
#define SEEN_RIGHT_L 400 // runs at full speed if it detects anything. 
#define SEEN_RIGHT_R 400 // this is to avoid losing in situations where two bots are locked onto each other
#define SEEN_LEFT_L SEEN_RIGHT_R
#define SEEN_LEFT_R SEEN_RIGHT_L
#define LASTSEEN_RIGHT_L 400
#define LASTSEEN_RIGHT_R -100
#define LASTSEEN_LEFT_L LASTSEEN_RIGHT_R
#define LASTSEEN_LEFT_R LASTSEEN_RIGHT_L
#define FIRST_TIME_ROTATE 400
#define SEARCH_ROTATE 400
#define SEARCH_FORWARD 400

#define IR_AVOID_L 400
#define IR_AVOID_R 100
#define IR_AVOID_DURATION 400 //ms


#define NUM_SENSORS 6

#define QTR_THRESHOLD  1500

#define MAX_DISTANCE 50 // this does not define the actual max distance of the sensors, as they are on another arduino

#define LASTSEEN_INTERVAL 750 // how long to remember having seen anything, in ms

#define DO_QTR true
#define DO_SEARCH true

unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

volatile unsigned int sonarR_distance;
volatile unsigned int sonarL_distance;

char lastSeen;
unsigned long lastSeenTimer;

bool seen;
bool trueSeen;
bool firstTime;

int turnDir;
unsigned long avoidEdgeReverseingTimer;
unsigned long avoidEdgeTurningTimer;


void setup() {
    Serial.begin(9600);
    sensors.init();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    seen=false;
    firstTime = true;
    lastSeenTimer = 0;
    avoidEdgeReverseingTimer = 0;
    avoidEdgeTurningTimer = 0;
    button.waitForButton();
}

void receiveEvent(int bytes) {
    sonarR_distance = Wire.read();
    sonarL_distance = Wire.read();
}

void loop() {

    if (lastSeen !='N' && (millis()-lastSeenTimer>LASTSEEN_INTERVAL)) {
        lastSeen = 'N';
        lastSeenTimer = 0;
    }

    sensors.read(sensor_values);

    int left_qtr;
    int right_qtr;
    if(!trueSeen){
        left_qtr = 0;
        right_qtr = 5; 
    }
    else{
        left_qtr = 2;
        right_qtr = 3;
    }

    if(avoidEdgeReverseingTimer>millis()){
        motors.setSpeeds(-REVERSE_SPEED_QTR, -REVERSE_SPEED_QTR);
    }
    else if(avoidEdgeTurningTimer>millis()){
        motors.setSpeeds(turnDir * TURN_SPEED_QTR, -turnDir * TURN_SPEED_QTR);
    }
    else if (DO_QTR && sensor_values[left_qtr] < QTR_THRESHOLD){
        turnDir = 1;
        avoidEdgeReverseingTimer = millis() + REVERSE_DURATION_QTR;
        avoidEdgeTurningTimer = millis() + REVERSE_DURATION_QTR + TURN_DURATION_QTR;
    } 
    else if (DO_QTR && sensor_values[right_qtr] < QTR_THRESHOLD) {
        turnDir = -1;
        avoidEdgeReverseingTimer = millis() + REVERSE_DURATION_QTR;
        avoidEdgeTurningTimer = millis() + REVERSE_DURATION_QTR + TURN_DURATION_QTR;
    } 
    else if (DO_SEARCH){
        search();
    }
    else{
        motors.setSpeeds(0, 0);
    }
}

        
void search(){

    bool see_right = (sonarR_distance < MAX_DISTANCE && sonarR_distance > 0);
    bool see_left = (sonarL_distance < MAX_DISTANCE && sonarL_distance > 0);
    seen = see_right || see_left;

    firstTime = firstTime && !seen; // firsttime resets when we see anything

    if (see_right && see_left && abs(sonarR_distance - sonarL_distance) < 5) {
        lastSeen = 'N';
        motors.setSpeeds(SEEN_BOTH,SEEN_BOTH);
    } 
    else if (see_right) {
        lastSeen = 'R';
        motors.setSpeeds(SEEN_RIGHT_L,SEEN_RIGHT_R);
        lastSeenTimer = millis(); // this "resets" the time until we throw away the lastseen data
    } 
    else if (see_left) {
        lastSeen = 'L';
        motors.setSpeeds(SEEN_LEFT_L,SEEN_LEFT_R);
        lastSeenTimer = millis(); // this "resets" the time until we throw away the lastseen data
    } 
    else if (lastSeen == 'R') {
        // motors.setSpeeds(LASTSEEN_RIGHT_L,LASTSEEN_RIGHT_R);
        // this change makes the bot spin in the direction it last saw something, instead of trying to follow like it did before
        motors.setSpeeds(SEARCH_ROTATE, -SEARCH_ROTATE);
    } 
    else if (lastSeen == 'L') {
        // motors.setSpeeds(LASTSEEN_LEFT_L,LASTSEEN_LEFT_R);
        // this change makes the bot spin in the direction it last saw something, instead of trying to follow like it did before
        motors.setSpeeds(-SEARCH_ROTATE, SEARCH_ROTATE);
    }
    else if (firstTime) {
        motors.setSpeeds(FIRST_TIME_ROTATE, -FIRST_TIME_ROTATE);
    }
    else if (millis() % 500 < 250){ // with no info we rotate half the time, and move forward the other half
        motors.setSpeeds(SEARCH_ROTATE, -SEARCH_ROTATE);
    }
    else {
        motors.setSpeeds(SEARCH_FORWARD, SEARCH_FORWARD); 
    }
}

