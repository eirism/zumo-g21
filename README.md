Examles are to be saved in the examples\ folder

The zumo-bot sketch is the main program, it should always work on master.

IR-sensor:
Top view - led to the right
Sensor | 5V
GND | GND


Behaviour:
Check if over line/edge, if yes: back up, return;
Check accelerometer (standing still). If movement, ESCAPE!, return;
Check if ir-front senses anything. If yes, turn against it and speed up, return;
Check ultrasonic front, speed up and turn against detected object, return;
Check ultrasonic back, turn around if detected (180 dgr), return;
Turn around while scanning. If turned one revolution, stop and drive for a while, return;
