#include <Wire.h>
#include <NewPing.h>

#include <Utils.h>

#define TRIGGER_PIN_SONAR_R 3
#define TRIGGER_PIN_SONAR_L 6

#define ECHO_PIN_R 2
#define ECHO_PIN_L 7
#define RED_LED 10
#define YELLOW_LED 11

#define MAX_DISTANCE 50

#define SONAR_INTERVAL 10

#define RESET_TIMEOUT 1
#define IR_THRESHOLD 800

Utils utils(RESET_TIMEOUT, IR_THRESHOLD);

NewPing sonarR(TRIGGER_PIN_SONAR_R, ECHO_PIN_R, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarL(TRIGGER_PIN_SONAR_L, ECHO_PIN_L, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sonarR_distance;
unsigned int sonarL_distance;

#define N_MEASUREMENTS_SONAR 5

unsigned int sonarR_array[N_MEASUREMENTS_SONAR];
unsigned int sonarL_array[N_MEASUREMENTS_SONAR];
int sonarR_pointer;
int sonarL_pointer;


#define IR A0
#define IRDIFF 10 // very sensitive to changes in light in the enviroment

unsigned int ir_distance;

#define N_MEASUREMENTS_IR 10

unsigned int ir_array[N_MEASUREMENTS_IR];
//int ir_pointer;

unsigned int irBaseline;

unsigned long timer;

void setup() {
    Serial.begin(9600);
    Wire.begin(); 
    timer=0;
    for(int i=0; i<N_MEASUREMENTS_SONAR; i++){
        sonarR_array[i] = 0;
        sonarL_array[i] = 0;
    }
    sonarR_pointer = 0;
    sonarL_pointer = 0;
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    //for(int i=0; i<N_MEASUREMENTS_IR; i++){
        //ir_array[i] = analogRead(IR);
        //delay(100);
    //}
    //irBaseline = median(ir_array, N_MEASUREMENTS_IR);
}

unsigned int median(unsigned int array[], int n) {
    if(n==0){
        return array[0];
    }
    unsigned int temp;
    int i, j;
    unsigned int x[n];
    for(i=0; i<n; i++){
        x[i] = array[i];
    }
    // the following two loops sort the array x in ascending order
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // swap elements
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // if there is an even number of elements, return mean of the two elements in the middle
        return((x[n/2] + x[n/2 - 1]) / 2);
    } else {
        // else return the element in the middle
        return x[n/2];
    }
}

void loop() {
    //ir_array[ir_pointer] = analogRead(IR);
    if((millis()-timer)>=SONAR_INTERVAL){  // runs at 50Hz
        timer=millis();
        sonarR_array[sonarR_pointer] = sonarR.ping_cm();
        sonarL_array[sonarL_pointer] = sonarL.ping_cm();

        sonarR_distance = median(sonarR_array, N_MEASUREMENTS_SONAR);
        sonarL_distance = median(sonarL_array, N_MEASUREMENTS_SONAR);
        ir_distance = 0;//analogRead(IR);//median(ir_array, N_MEASUREMENTS_IR);
        //Serial.println(ir_distance);

        Wire.beginTransmission(9); 
        Wire.write(sonarR_distance);            
        Wire.write(sonarL_distance);            
        Wire.write(false); //ir_distance<(irBaseline-IRDIFF));            
        Wire.endTransmission();

        digitalWrite(RED_LED, sonarR_distance > 0);
        digitalWrite(YELLOW_LED, sonarL_distance > 0);

        bool right_bad = (sonarR_array[sonarR_pointer] == 0 ) && digitalRead(ECHO_PIN_R) == HIGH;
        bool left_bad = (sonarL_array[sonarL_pointer] == 0 ) && digitalRead(ECHO_PIN_L) == HIGH;

        if (right_bad && left_bad) {
            utils.resetEcho(ECHO_PIN_R, ECHO_PIN_L);   
        }
        else if (right_bad) {
            utils.resetEcho(ECHO_PIN_R);   
        }
        else if (left_bad) {
            utils.resetEcho(ECHO_PIN_L);   
        } 
        sonarR_pointer += 1;
        sonarR_pointer %= N_MEASUREMENTS_SONAR;
        sonarL_pointer += 1;
        sonarL_pointer %= N_MEASUREMENTS_SONAR;

        Serial.println(millis()-timer);
    }
    //ir_pointer += 1;
    //ir_pointer %= N_MEASUREMENTS_IR;
}
