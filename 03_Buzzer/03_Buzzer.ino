#include <MeMCore.h>

MeBuzzer buzzer;

void setup() {

  buzzer.tone(600, 1000);   //Buzzer sounds 600Hz for 1000ms
  delay(2000);              //Pause for 2000ms, Buzzer no sound
  buzzer.tone(1200, 1000);  //Buzzer sounds 1200Hz for 1000ms
  delay(2000);              //Pause for 2000ms, Buzzer no sound
}

void loop(){}