/*State-Diagram: SWITCHING BETWEEN CONTROL ALGORITHMS
    start
      |                   +-------\
      V                   V       |    
  --------              ------    |
 |  OFF   |<--click-+->|  ON  |   |    Bright WHITE 
  --------          |   ------    |    
 Light WHITE        |     |       |
 on the first start |double-click |
                    |     |       |
        |     V       |
                    |   -------   |
                    +- | BASIC |  |    RED & GREEN
                    |   -------   |
                    |     |       |
                    |double-click |        - RED on the side  is OUT
                    |     |       |        - GREEN on the side is IN
                    |     V       |
                    |   ------    |
                    +--|  PI  |---/    ORANGE & BLUE
                        ------ 
 */
 
 // STATES -----------------------------------------------------------
typedef enum {
  _OFF,   // stop Robot
  _ON,    // start Robot with control loop of state _BASIC
  _BASIC, // Basic loop control: turn RIGHT on left side, turn LEFT on righ side, move FORWARD on line
  _PI     // Pseudo-PI control (proportional-integral)
} 
States;

States nextState = _OFF; // Robot stopped when starting

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
// -----------------------
int moveSpeed = 255*0.5; // Velocidad de referencia //230 in ref Program
int moveSpeedTurn = 255; // Velocidad de referencia //230 in ref Program
float SIG1 = -1.0; //Left motor scale factor, negative for going forward
float SIG2 = 1.0;  //Right motor scale factor
// ----------------------------------------------------------------------------

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

 switch (nextState) {
    case _OFF:
      // Stop the robot
      Serial.println("_OFF state' mBot stopping"); /////////////////////////////////// SERIAL MONITOR
      motor1.run(SIG1 * 0);  //Left motor Stop
      motor2.run(SIG2 * 0);  //Right motor Stop
      led.setColorAt(1, 50, 50, 50); //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 50, 50, 50); //Set LED0 (RGBLED1) (RightSide)
      led.show();
      break;

    case _ON:
      // Start (or re-start) the robot
      motor1.run(SIG1 * 0);  //Left motor Stop
      motor2.run(SIG2 * 0);  //Right motor Stop
      Serial.println("'_ON state' mBot ready to go"); ///////////////////////////////// SERIAL MONITOR
      led.setColorAt(1, 0, 0, 255); //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 0, 0, 255); //Set LED0 (RGBLED1) (RightSide)
      led.show();
      break;

    case _BASIC:
    Serial.println("_BASIC state' Using basic line following algorithm"); ///////////// SERIAL MONITOR
      // Basic line following algorithm
      FollowLineBasic (sensorState);
      break;

    case _PI:
    Serial.println("_PI state' Using PI line following algorithm"); /////////////////// SERIAL MONITOR
      // PI line following algorithm
      FollowLinePI (sensorState);
      break;
      
    default: break; }
  
  } // End of loop()
/***************************************************************************************************/

//////// FUNCTION FOR CONTROL LOOP algorithm: Basic *************************
  void FollowLineBasic (int sensorState) {
    static float turning_left = true;
    
    switch (sensorState) {
      case S1_IN_S2_IN:         // go FORWARD 
        motor1.run(SIG1 * moveSpeed); //Left motor Run
        motor2.run(SIG2 * moveSpeed); //Right motor Run
        led.setColorAt(1, 0, 255, 0); //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 0, 255, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
      
      case S1_IN_S2_OUT:         // turn LEFT 
        motor1.run(SIG1 * 0);           //Left motor Stop
        motor2.run(SIG2 * moveSpeed);   //Right motor Run        
        led.setColorAt(1, 0, 255, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        turning_left = true;
        break;

      case S1_OUT_S2_IN:         //turn RIGHT
        motor2.run(SIG2 * 0);         //Right motor Stop
        motor1.run(SIG1 * moveSpeed);  //Left motor Run
        led.setColorAt(1, 255, 0, 0); //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 0, 255, 0);   //Set LED0 (RGBLED1) (RightSide)
        led.show();
        turning_left = false;
        break;

      case S1_OUT_S2_OUT:
         // keep turning what it was turning
        if (turning_left) {
          motor1.run(SIG1 * 0);      //Left motor Stop
          motor2.run(SIG2 * moveSpeed);  //Right motor Run
        } else {
          motor1.run(SIG1 * moveSpeed);  //Left motor Run
          motor2.run(SIG2 * 0);      //Right motor Stop
        }
        led.setColorAt(1, 255, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
    
      default: break; } // End of 'switch' statement
  
  } // End of function FollowLineBasic

  //////// FUNCTION FOR CONTROL LOOP algorithm: PI *************************
  void FollowLinePI (int sensorState) {
    static int LineFollowFlag=0;  // Integral part of PI control loop
    
    switch (sensorState) {
      case S1_IN_S2_IN:         // go FORWARD    
        motor1.run(SIG1 * moveSpeed); //230 in ref Program (repo from 'picasa')
        motor2.run(SIG2 * moveSpeed);
        LineFollowFlag=10; // ------------------------------------------- RESETS THE INTEGRAL PART   
        led.setColorAt(1, 0, 0, 255); //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 0, 0, 255); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
      
      case S1_IN_S2_OUT:         // go FORWARD
        motor1.run(SIG1 * moveSpeed);
        motor2.run(SIG2 * moveSpeed);
        if (LineFollowFlag>1) LineFollowFlag--; // ---------------- DECREASE on RIGHT SIDE of line           
        led.setColorAt(1,  0,  0, 255);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 255, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;

      case S1_OUT_S2_IN:         // go FORWARD
        motor1.run(SIG1 * moveSpeed);
        motor2.run(SIG2 * moveSpeed);
        if (LineFollowFlag<20) LineFollowFlag++; // ---------------- INCREASE on LEFT SIDE of line
        led.setColorAt(1, 255, 255, 0); //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0,  0,  0, 255);   //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;

      case S1_OUT_S2_OUT:       // if 10 backward, <10 turn left, >10 turn right
        if(LineFollowFlag==10){
          //Backward(); // if defined inside a function
          motor1.run(-SIG1 * moveSpeed); // Malfunctioning possibly due to delay(200) in 'function onBoardButton_tick'
          motor2.run(-SIG2 * moveSpeed); //  "
          }
        if(LineFollowFlag<10){
          //TurnLeft(); // if defined inside a function
          motor1.run(SIG1 * moveSpeedTurn/10);
          motor2.run(SIG2 * moveSpeedTurn);
          }
        if(LineFollowFlag>10){
          //TurnRight(); // if defined inside a function
          motor1.run(SIG1 * moveSpeedTurn);
          motor2.run(SIG2 * moveSpeedTurn/10);
          }
        led.setColorAt(1, 255, 255, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 255, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
    
      default: break; } // End of 'switch' statement
  
  } // End of function FollowLinePI

// TRANSITION FUNCTIONS ----------------------------------------------
// On LONG PRESS: empty function, not used
void myPressFunction() {
  // Nothing to do  
}

// On SINGLE CLICK: this function will be called when the button was pressed 1 time and them some time has passed.
void myClickFunction() {
  if (nextState == _OFF) {
    nextState = _ON;
  }
  else
    nextState = _OFF;
  }

// On DOUBLE CLICK: this function will be called when the button was pressed 2 times in a short timeframe.
void myDoubleClickFunction() {
  
  if (nextState == _ON) {
    nextState = _BASIC;

  } else if (nextState == _BASIC) {
    nextState = _PI;

  } else if (nextState == _PI) {
    nextState = _ON;
  } // if
}

