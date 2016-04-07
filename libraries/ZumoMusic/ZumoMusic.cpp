#include <ZumoBuzzer.h>
#include <Arduino.h>
#include "ZumoMusic.h"


ZumoMusic::ZumoMusic() {
  pinMode(3, OUTPUT);
 to_wait = 0;

pos = 0;

toggle = false;
prev = true;



playNotes = notes;
playDelays = delays;
arraySize = sizeof(notes); randomSeed(millis());
  chooseMusic();
}

double ZumoMusic::tone_wrapper(int note, int dur) {
  tone(3, note, dur);
  return dur + 1;
}

// the loop routine runs over and over again forever:
void ZumoMusic::loop() {
  //tone(pin, note, duration)
  long curMillis = millis();
  if (curMillis - prevTone >= to_wait) {
    to_wait = tone_wrapper(playNotes[pos], playDelays[pos]);
    prevTone = curMillis;
    pos++;
  }
  if (pos > arraySize / sizeof(int)) {
    pos = 0;
    chooseMusic();
  }
}

void ZumoMusic::chooseMusic() {
  int choice = random(1, 3);
    switch (choice) {
      case 1:
        playNotes = notes;
        playDelays = delays;
        arraySize = sizeof(notes);
        break;
      case 2:
        playNotes = marioNotes;
        playDelays = marioDelays;
        arraySize = sizeof(marioNotes);
        break;
    }
}

