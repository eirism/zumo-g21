#include <Wire.h>
#include <NewPing.h>

#include <Utils.h>

#define TRIGGER_PIN_SONAR_R 3
#define TRIGGER_PIN_SONAR_C 5
#define TRIGGER_PIN_SONAR_L 6
#define ECHO_PIN_R 2
#define ECHO_PIN_C 4
#define ECHO_PIN_L 7
#define MAX_DISTANCE 70

#define SONAR_INTERVAL 50

Utils utils;

NewPing sonarR(TRIGGER_PIN_SONAR_R, ECHO_PIN_R, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarC(TRIGGER_PIN_SONAR_C, ECHO_PIN_C, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
NewPing sonarL(TRIGGER_PIN_SONAR_L, ECHO_PIN_L, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int sonarR_distance;
unsigned int sonarC_distance;
unsigned int sonarL_distance;

unsigned long timer;

void setup() {
	Serial.begin(9600);
	Wire.begin(); 
	timer=0;
}


void loop() {
	if((millis()-timer)>=SONAR_INTERVAL){  // runs at 50Hz
		timer=millis();
		sonarR_distance = sonarR.ping_cm();
		sonarC_distance = sonarC.ping_cm();
		sonarL_distance = sonarL.ping_cm();

		Wire.beginTransmission(9); 
		Wire.write(sonarR_distance);            
		Wire.write(sonarC_distance);            
		Wire.write(sonarL_distance);            
		Wire.endTransmission();  

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
	}
}
