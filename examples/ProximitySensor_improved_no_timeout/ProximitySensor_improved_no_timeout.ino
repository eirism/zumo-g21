// From http://forum.arduino.cc/index.php?topic=55119.30

#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int cm = sonar.ping_cm();
  Serial.print("Ping: ");
  Serial.print(cm); // Send ping, get distance in cm and print result (0 = outside set distance range)
  Serial.println("cm");

  if (cm == 0 && digitalRead(ECHO_PIN) == HIGH) {
    pinMode(ECHO_PIN, OUTPUT);
    digitalWrite(ECHO_PIN, LOW);
    delay(50);
    pinMode(ECHO_PIN, INPUT);   
  }
}
