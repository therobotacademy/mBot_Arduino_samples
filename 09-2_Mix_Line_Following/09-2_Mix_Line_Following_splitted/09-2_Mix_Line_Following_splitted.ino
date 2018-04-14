#include "09-2_GlobalDefines.h"

// ----- On board button definitions ----------------------------------------
#include "OneButton.h"
#define RGB_LEDS 13
#define ON_BOARD_BUTTON A7
// -----------------------
// Setup a new OneButton on pin A7  
OneButton button(ON_BOARD_BUTTON, true);

// Makeblock definitions -----------------------------------------------------
#include <MeMCore.h>
MeBuzzer buzzer;
MeLineFollower lineFinder(PORT_2);
MeDCMotor motor1(M1); //Motor1 is Left Motor
MeDCMotor motor2(M2); //Motor2 is Left Motor
MeRGBLed led(0, 30);

/****************** SETUP *********************************************************************************/
void setup() {
  led.setpin(RGB_LEDS);
  led.setColorAt(1, 50, 50, 50); //Set LED1 (RGBLED2) (LeftSide)
  led.setColorAt(0, 50, 50, 50); //Set LED0 (RGBLED1) (RightSide)
  led.show();

  /////// OneButton callback functions
  // link the myClickFunction function to be called on a click event.   
  button.attachClick(myClickFunction);
  // link the doubleclick function to be called on a doubleclick event.   
  button.attachDoubleClick(myDoubleClickFunction);
  // link the myPressFunction function to be called on a long press event.   
  button.attachPress(myPressFunction);
  
  // set 80 msec. debouncing time. Default is 50 msec.
  button.setDebounceTicks(20);
  // set msec. before a click is detected. Default is 600 msec.
  button.setClickTicks(600);
  // set msec. before a click is detected. Default is 1000 msec.
  button.setPressTicks(1000);
  
  Serial.begin(9600);
  Serial.println("Press BUTTON to start"); /////////////////////////////////// SERIAL MONITOR
  buzzer.tone(200, 200);   //Buzzer beep to indicate _OFF state
}
/******************* LOOP ********************************************************************************/
void loop() {
  // -------- Check for press / click on board button ----------
  button.tick();  // From OneButton library
  
  // --------------------- MAKE TRANSITION (OneButton library) ---------------------
  // Logic is inside TRANSITION FUNCTIONS, that will be called from next switch statement
  
  // ---- EXECUTE THE TASK OF THE SWITCHED STATE ---------------
  int sensorState = lineFinder.readSensors();
  ExecuteTask(sensorState);
 
  } // End of loop()
/***************************************************************************************************/
