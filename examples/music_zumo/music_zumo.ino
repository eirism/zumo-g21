#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <PLab_ZumoMotors.h>
#include <ZumoReflectanceSensorArray.h>
#include <QTRSensors.h>

// NB: ALL NOTES DEFINED WITH STANDARD ENGLISH NAMES, EXCEPT FROM "A"
//THAT IS CALLED WITH THE ITALIAN NAME "LA" BECAUSE A0,A1...ARE THE ANALOG PINS ON ARDUINO.
// (Ab IS CALLED Ab AND NOT LAb)
#define C0 16
#define Db0 17
#define D0  18
#define Eb0 19
#define E0  21
#define F0  22
#define Gb0 23
#define G0  25
#define Ab0 26
#define LA0 28
#define Bb0 29
#define B0  31
#define C1  33
#define Db1 35
#define D1  37
#define Eb1 39
#define E1  41
#define F1  44
#define Gb1 46
#define G1  49
#define Ab1 52
#define LA1 55
#define Bb1 58
#define B1  62
#define C2  65
#define Db2 69
#define D2  73
#define Eb2 78
#define E2  82
#define F2  87
#define Gb2 93
#define G2  98
#define Ab2 104
#define LA2 110
#define Bb2 117
#define B2  123
#define C3  131
#define Db3 139
#define D3  147
#define Eb3 156
#define E3  165
#define F3  175
#define Gb3 185
#define G3  196
#define Ab3 208
#define LA3 220
#define Bb3 233
#define B3  247
#define C4  262
#define Db4 277
#define D4  294
#define Eb4 311
#define E4  330
#define F4  349
#define Gb4 370
#define G4  392
#define Ab4 415
#define LA4 440
#define Bb4 466
#define B4  494
#define C5  523
#define Db5 554
#define D5  587
#define Eb5 622
#define E5  659
#define F5  698
#define Gb5 740
#define G5  784
#define Ab5 831
#define LA5 880
#define Bb5 932
#define B5  988
#define C6  1047
#define Db6 1109
#define D6  1175
#define Eb6 1245
#define E6  1319
#define F6  1397
#define Gb6 1480
#define G6  1568
#define Ab6 1661
#define LA6 1760
#define Bb6 1865
#define B6  1976
#define C7  2093
#define Db7 2217
#define D7  2349
#define Eb7 2489
#define E7  2637
#define F7  2794
#define Gb7 2960
#define G7  3136
#define Ab7 3322
#define LA7 3520
#define Bb7 3729
#define B7  3951
#define C8  4186
#define Db8 4435
#define D8  4699
#define Eb8 4978
// DURATION OF THE NOTES

#define BPM 120    //  you can change this value changing all the others
// 120 for imperial march
// 240 for mario
#define H 2*Q //half 2/4
#define Q 60000/BPM //quarter 1/4 
#define E Q/2   //eighth 1/8
#define S Q/4 // sixteenth 1/16
#define W 4*Q // whole 4/4
#define BASE 2000
#define A BASE/12
#define B BASE/6
#define C BASE/18
#define D BASE/10
#define E BASE/9

int marioDelays[] = {
  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 

  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 

  E,E,E,
  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 

  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A, 

  E, E, E, 
  A,A,A,A, 
  A,A,A,A, 
  A,A,A,A};

int marioNotes[] = {
  E7,E7,0,E7, 
  0,C7,E7,0, 
  G7,0,0,0, 
  G6,0,0,0, 

  C7,0,0,G6, 
  0,0,E6,0, 
  0,LA6,0,B6, 
  0,Bb6,LA6,0, 

  G6,E7,G7,
  LA7,0,F7,G7, 
  0,E7,0,C7, 
  D7,B6,0,0, 

  C7,0,0,G6, 
  0,0,E6,0, 
  0,LA6,0,B6, 
  0,Bb6,LA6,0, 

  G6, E7, G7, 
  LA7,0,F7,G7, 
  0,E7,0,C7, 
  D7,B6,0,0};

long prevTone;

int to_wait = 0;

int notes[] = {LA3, LA3, LA3, F3, C4, LA3, F3, C4, LA3, E4, E4, E4, F4, C4, Ab3, F3, C4, LA3, LA4, LA3, LA3, LA4, Ab4, G4, Gb4, E4, F4, 0, Bb3, Eb4, D4, Db4, C4, B3, C4, 0, F3, Ab3, F3, LA3, C4, LA3, C4, E4, LA4, LA3, LA3, LA4, Ab4, G4, Gb4, E4, F4, 0, Bb3, Eb4, D4, Db4, C4, B3, C4, 0, F3, Ab3, F3, C4, LA3, F3, C4, LA3, 0};
int delays[] = {Q, Q, Q, E + S, S, Q, E + S, S, H, Q, Q, Q, E + S, S, Q, E + S, S, H, Q, E + S, S, Q, E + S, S, S, S, E, E, E, Q, E + S, S, S, S, E, E, E, Q, E + S, S, Q, E + S, S, H, Q, E + S, S, Q, E + S, S, S, S, E, E, E, Q, E + S, S, S, S, E, E, E, Q, E + S, S, Q, E + S, S, H, 2 * H};

int pos = 0;

int *playNotes = notes;
int *playDelays = delays;
int arraySize = sizeof(notes);

int buzzerPin = 3;
int buttonPin = 2;

int toggle = LOW;
int prev = HIGH;

Pushbutton button(ZUMO_BUTTON);

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);
  button.waitForButton();
  randomSeed(millis());
  chooseMusic();
}

double tone_wrapper(int pin, int note, int dur) {
  tone(pin, note, dur);
  return dur + 1;
}

// the loop routine runs over and over again forever:
void loop() {
  //tone(pin, note, duration)
  long curMillis = millis();
  if (curMillis - prevTone >= to_wait) {
    to_wait = tone_wrapper(buzzerPin, playNotes[pos], playDelays[pos]);
    prevTone = curMillis;
    pos++;
  }
  if (pos > arraySize / sizeof(int)) {
    pos = 0;
    chooseMusic();
  }
}

void chooseMusic() {
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


