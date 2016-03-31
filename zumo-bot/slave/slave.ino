#include <Wire.h>
#include <NewPing.h>

#include <Utils.h>

#define TRIGGER_PIN_SONAR_R 3
#define TRIGGER_PIN_SONAR_L 6

#define ECHO_PIN_R 2
#define ECHO_PIN_L 7

#define MAX_DISTANCE 70

#define SONAR_INTERVAL 50

#define RESET_TIMEOUT 50
#define IR_THRESHOLD 800

Utils utils(RESET_TIMEOUT, IR_THRESHOLD);

NewPing sonarR(TRIGGER_PIN_SONAR_R, ECHO_PIN_R, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarL(TRIGGER_PIN_SONAR_L, ECHO_PIN_L, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sonarR_distance;
unsigned int sonarL_distance;

#define N_MEASUREMENTS 5

unsigned int sonarR_array[N_MEASUREMENTS];
unsigned int sonarL_array[N_MEASUREMENTS];
int sonarR_pointer;
int sonarL_pointer;

unsigned int temp[N_MEASUREMENTS];

unsigned long timer;

void setup() {
	Serial.begin(9600);
	Wire.begin(); 
	timer=0;
    for(int i=0; i<N_MEASUREMENTS; i++){
    	sonarR_array[i] = 0;
    	sonarL_array[i] = 0;
    }
	sonarR_pointer = 0;
	sonarL_pointer = 0;
  pinMode(10, OUTPUT); //
  pinMode(11, OUTPUT); //
}
/*
unsigned int averageValue(unsigned int[] array){
	unsigned int mini = -1;
	for(int i=0; i<6; i++){
		for(int j=0; j<6; j++){
			mini = mini < array[j] ? mini : array[j];
			temp[i] = array[i];
	}
	return (temp[2]+temp[3])/2
}
*/
unsigned int median(unsigned int array[]) {
    unsigned int temp;
    int i, j;
    int n = N_MEASUREMENTS;
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
	if((millis()-timer)>=SONAR_INTERVAL){  // runs at 50Hz
		timer=millis();
		sonarR_array[sonarR_pointer] = sonarR.ping_cm();
		sonarL_array[sonarL_pointer] = sonarL.ping_cm();

		sonarR_distance = median(sonarR_array);
		sonarL_distance = median(sonarL_array);

		Wire.beginTransmission(9); 
		Wire.write(sonarR_distance);            
		Wire.write(sonarL_distance);            
		Wire.endTransmission();
    if (sonarR_distance > 0){
      digitalWrite(10, HIGH);
    }else{
      digitalWrite(10, LOW);
    }
    if (sonarL_distance > 0){
      digitalWrite(11, HIGH);
    }else{
      digitalWrite(11, LOW);
    }

		Serial.print(sonarR_distance);
		Serial.print(" ");
		//Serial.print(sonarC_distance);
		//Serial.print(" ");
		Serial.println(sonarL_distance);

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
		sonarR_pointer %= N_MEASUREMENTS;
		sonarL_pointer += 1;
		sonarL_pointer %= N_MEASUREMENTS;
	}
}
