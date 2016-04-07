#include <ZumoMusic.h>
#include <Pushbutton.h>

Pushbutton button(ZUMO_BUTTON);
ZumoMusic music;

void setup(){
  music = ZumoMusic();
  button.waitForButton();
}


void loop(){
  music.loop();

}
