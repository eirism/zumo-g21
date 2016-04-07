//#define PLAY_MUSIC

#ifdef PLAY_MUSIC 
#include <ZumoMusic.h>
#endif
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

#define IR_AVOID_L 400
#define IR_AVOID_R 100
#define IR_AVOID_DURATION 400 //ms


#define NUM_SENSORS 6

#define QTR_THRESHOLD  1500

#define MAX_DISTANCE 70

#define LASTSEEN_INTERVAL 500

#define DO_QTR false
#define DO_IR true
#define DO_SEARCH false

unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);

volatile unsigned int sonarR_distance;
volatile unsigned int sonarL_distance;

char lastSeen;

unsigned long timer;

bool seen;
bool firstTime;

volatile bool ir_active=false;

int turnDir;
unsigned long avoidEdgeReverseingTimer;
unsigned long avoidEdgeTurningTimer;

unsigned long irAvoidTimer;

#ifdef PLAY_MUSIC 
ZumoMusic music;
#endif

void setup() {
    Serial.begin(9600);
    sensors.init();
    Wire.begin(9);
    Wire.onReceive(receiveEvent);
    pinMode(GREEN_LED, OUTPUT); // indicator for ir_active
    seen=false;
    firstTime = true;
    timer = 0;
    avoidEdgeReverseingTimer = 0;
    avoidEdgeTurningTimer = 0;
    irAvoidTimer = 0;

    button.waitForButton();
    #ifdef PLAY_MUSIC
    music = ZumoMusic();
    #endif
}

void receiveEvent(int bytes) {
    sonarR_distance = Wire.read();
    sonarL_distance = Wire.read();
    ir_active = Wire.read();
}

void loop() {

#ifdef PLAY_MUSIC
      music.loop();
#endif

    if (lastSeen !='N' && (millis()-timer>LASTSEEN_INTERVAL)) {
        lastSeen = 'N';
        timer = 0;
    }

    sensors.read(sensor_values);

    digitalWrite(GREEN_LED, ir_active);

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
        irAvoidTimer = 0;
    } 
    else if (DO_QTR && sensor_values[right_qtr] < QTR_THRESHOLD) {
        turnDir = -1;
        avoidEdgeReverseingTimer = millis() + REVERSE_DURATION_QTR;
        avoidEdgeTurningTimer = millis() + REVERSE_DURATION_QTR + TURN_DURATION_QTR;
        irAvoidTimer = 0;
    } 
    else if(irAvoidTimer>millis()){
        motors.setSpeeds(IR_AVOID_L, IR_AVOID_R);
    }
    else if(DO_IR && ir_active){
        irAvoidTimer = millis() + IR_AVOID_DURATION;
    }
    else if (DO_SEARCH){
        search();
    }
    else{
        motors.setSpeeds(0, 0);
    }
}
void avoid_edge(int dir){// no longer need this function
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

