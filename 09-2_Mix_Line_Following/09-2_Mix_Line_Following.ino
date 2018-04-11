/*State-Diagram: SWITCHING BETWEEN CONTROL ALGORYTHMS
    start
      |                   +-------\
      V                   V       |
  --------              ------    |
 |  OFF   |<--click-+->|  ON  |   |    RED on the side  is OUT
  --------          |   ------    |    GREEN on the side is IN
 WHITE (RGD leds)   |     |       |
 on the first start |double-click |
                    |     |       |
 BLUE on stopping   |     V       |
                    |   -------   |
                    +- | BASIC |  |
                    |   -------   |
                    |     |       |
                    |double-click |
                    |     |       |
                    |     V       |
                    |   ------    |
                    +--|  PI  |---/
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
int STOP_moving = 0; //***************************************

// TRANSITION FUNCTIONS ----------------------------------------------
// On LONG PRESS: empty function, not used
void myPress() {
  
}

// On SINGLE CLICK: this function will be called when the button was pressed 1 time and them some time has passed.
void myClickFunction() {
  if (nextState == _OFF)
    nextState = _ON;
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
    nextState = _BASIC;
  } // if
}

// ----- On board button definitions ----------------------------------------
#include "OneButton.h"
#define RGB_LEDS 13
#define ON_BOARD_BUTTON A7
#define READ_THRESHOLD 100
// -----------------------
// Setup a new OneButton on pin A7  
OneButton button(A7, true);
boolean currentPressed = false;
boolean pre_buttonPressed = false;
unsigned int button_read = 1023;  // Starting value is "released" = HIGH (as opposite to "pressed"  LOW)

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
  led.setColorAt(1, 255, 255, 255); //Set LED1 (RGBLED2) (LeftSide)
  led.setColorAt(0, 255, 255, 255); //Set LED0 (RGBLED1) (RightSide)
  led.show();
  
  pinMode(ON_BOARD_BUTTON, INPUT); //Define button pin as input
  Serial.begin(9600);

  while (button_read > 100) {
    delay(50); //Wait till button pressed to start
    button_read = analogRead(ON_BOARD_BUTTON);
  }
  buzzer.tone(200, 200);   //Buzzer beep to indicate start
}
/******************* LOOP ********************************************************************************/
void loop() {
  // -------- Check for press / click on board button ----------
  unsigned long now = millis();
  // keep watching the push button:
  button.tick();
  // --------------------- MAKE TRANSITION ---------------------
  // Logic is inside TRANSITION FUNCTIONS, that will be called from next switch statement
  
  // ---- EXECUTE THE TASK OF THE SWITCHED STATE ---------------
  int sensorState = lineFinder.readSensors();
  
  switch (nextState) {
    case _OFF:
      // Stop the robot
      Serial.println("mBot stopping"); /////////////////////////////////// SERIAL MONITOR
      motor1.run(SIG1 * 0);  //Left motor Stop
      motor2.run(SIG2 * 0);  //Right motor Stop
      led.setColorAt(1, 0, 0, 255); //Set LED1 (RGBLED2) (LeftSide) BLUE color
      led.setColorAt(0, 0, 0, 255); //Set LED0 (RGBLED1) (RightSide) BLUE color
      led.show();
      break;

    case _ON:
      // Start (or re-start) the robot
      motor1.run(SIG1 * 0);  //Left motor Stop
      motor2.run(SIG2 * 0);  //Right motor Stop
      Serial.println("mBot moving in 2 seconds..."); ///////////////////////////////////// SERIAL MONITOR
      led.setColorAt(1, 255, 0, 255); //Set LED1 (RGBLED2) (LeftSide) VIOLET color
      led.setColorAt(0, 255, 0, 255); //Set LED0 (RGBLED1) (RightSide) VIOLET color
      led.show();
      delay(2000);
      break;

    case _BASIC:
      // Basic line following algorythm
      FollowLineBasic (sensorState);
      break;

    case _PI:
      // PI line following algorythm
      FollowLinePI (sensorState);
      break;
      
    default: break; }
  } // End of loop()
/***************************************************************************************************/

//////// FUNCTION FOR CONTROL LOOP ALGORYTHM: Basic *************************
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
          motor2.run(SIG2 * 255.0);  //Right motor Run
        } else {
          motor1.run(SIG1 * 255.0);  //Left motor Run
          motor2.run(SIG2 * 0);      //Right motor Stop
        }
        led.setColorAt(1, 255, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
    
      default: break; } // End of 'switch' statement
  
  } // End of function FollowLineBasic

  //////// FUNCTION FOR CONTROL LOOP ALGORYTHM: PI *************************
  void FollowLinePI (int sensorState) {
    static int LineFollowFlag=0;  // Integral part of PI control loop
    
    switch (sensorState) {
      case S1_IN_S2_IN:         // go FORWARD    
        motor1.run(SIG1 * moveSpeed); //230 in ref Program (repo from 'picasa')
        motor2.run(SIG2 * moveSpeed);
        LineFollowFlag=10; // ------------------------------------------- RESETS THE INTEGRAL PART   
        led.setColorAt(1, 0, 255, 0); //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 0, 255, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
      
      case S1_IN_S2_OUT:         // go FORWARD
        motor1.run(SIG1 * moveSpeed);
        motor2.run(SIG2 * moveSpeed);
        if (LineFollowFlag>1) LineFollowFlag--; // ---------------- DECREASE on RIGHT SIDE of line           
        led.setColorAt(1, 0, 255, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;

      case S1_OUT_S2_IN:         // go FORWARD
        motor1.run(SIG1 * moveSpeed);
        motor2.run(SIG2 * moveSpeed);
        if (LineFollowFlag<20) LineFollowFlag++; // ---------------- INCREASE on LEFT SIDE of line
        led.setColorAt(1, 255, 0, 0); //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 0, 255, 0);   //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;

      case S1_OUT_S2_OUT:       // if 10 backward, <10 turn left, >10 turn right
        if(LineFollowFlag==10){
          //Backward(); // if defined inside a function
          motor1.run(-SIG1 * moveSpeed);
          motor2.run(-SIG2 * moveSpeed);
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
        led.setColorAt(1, 255, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
        break;
    
      default: break; } // End of 'switch' statement
  
  } // End of function FollowLinePI

