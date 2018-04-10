#include <MeMCore.h>

#define RGB_LEDS 13
#define ON_BOARD_BUTTON A7
#define READ_THRESHOLD 100
boolean currentPressed = false;
boolean pre_buttonPressed = false;
unsigned int button_read = 1023;  // Starting value is "released" (in opposition to "pressed")
boolean STOP_moving = false;

MeBuzzer buzzer;
MeLineFollower lineFinder(PORT_2);
MeDCMotor motor1(M1); //Motor1 is Left Motor
MeDCMotor motor2(M2); //Motor2 is Left Motor
MeRGBLed led(0, 30);

int LineFollowFlag=0;  // Integral part of PI control loop
int moveSpeed = 255*0.5; // Velocidad de referencia //230 in ref Program
int moveSpeedTurn = 255; // Velocidad de referencia //230 in ref Program

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

float MOTOR1_TUNE = -1.0; //Left motor scale factor, negative for going forward
float MOTOR2_TUNE = 1.0;  //Right motor scale factor
float turning_left = true;

void loop() {
  // Update status of BOARD BUTTON
  currentPressed = (analogRead(ON_BOARD_BUTTON) < READ_THRESHOLD);
  if (currentPressed != pre_buttonPressed)
  {
    pre_buttonPressed = currentPressed;
    if (currentPressed == true)
    {
      // Change state of Moving to Stop or viceversa when pressing the Onboard Button
      STOP_moving = !STOP_moving;
      if (!STOP_moving) Serial.println("mBot stopping");
        else            Serial.println("mBot moving");
    }
  }

  if (STOP_moving)  // Stop the robot when button is pressed
  {
    motor1.run(MOTOR1_TUNE * 0);  //Left motor Stop
    motor2.run(MOTOR2_TUNE * 0);  //Right motor Stop
    led.setColorAt(1, 0, 0, 255); //Set LED1 (RGBLED2) (LeftSide)
    led.setColorAt(0, 0, 0, 255); //Set LED0 (RGBLED1) (RightSide)
    led.show();
  }
  else   // Else follow line
  {  
  Serial.println(LineFollowFlag);
  int sensorState = lineFinder.readSensors();
  switch (sensorState)
  {
    case S1_IN_S2_IN:
      motor1.run(MOTOR1_TUNE * moveSpeed); //230 in ref Program. Left motor Run. Half of maximum speed to avoid both sensor out
      motor2.run(MOTOR2_TUNE * moveSpeed); //Right motor Run
      LineFollowFlag=10; // ------------------------------------------- RESETS THE INTEGRAL PART
      
      led.setColorAt(1, 0, 255, 0); //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 0, 255, 0); //Set LED0 (RGBLED1) (RightSide)
      led.show();
      break;
    case S1_IN_S2_OUT:
      /*turn LEFT
      motor1.run(MOTOR1_TUNE * 0);      //Left motor Stop
      */
      // Case of PI control: go FORWARD
      motor1.run(MOTOR1_TUNE * moveSpeed);
      motor2.run(MOTOR2_TUNE * moveSpeed);  //Right motor Run
      if (LineFollowFlag>1) LineFollowFlag--; // ---------------- DECREASE on RIGHT SIDE of line
            
      led.setColorAt(1, 0, 255, 0);   //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
      led.show();
      turning_left = true;
      break;
    case S1_OUT_S2_IN:
      /*turn RIGHT
      motor2.run(MOTOR2_TUNE * 0);      //Right motor Stop
      */
      // Case of PI control: go FORWARD
      motor1.run(MOTOR1_TUNE * moveSpeed);  //Left motor Run
      motor2.run(MOTOR2_TUNE * moveSpeed);
      if (LineFollowFlag<20) LineFollowFlag++; // ---------------- INCREASE on LEFT SIDE of line

      led.setColorAt(1, 255, 0, 0); //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 0, 255, 0);   //Set LED0 (RGBLED1) (RightSide)
      led.show();
      turning_left = false;
      break;
    case S1_OUT_S2_OUT:
        if(LineFollowFlag==10){
          //Backward();
          motor1.run(-MOTOR1_TUNE * moveSpeed);
          motor2.run(-MOTOR2_TUNE * moveSpeed);
          }
        if(LineFollowFlag<10){
          //TurnLeft();
          motor1.run(MOTOR1_TUNE * moveSpeedTurn/10);
          motor2.run(MOTOR2_TUNE * moveSpeedTurn);
          }
        if(LineFollowFlag>10){
          //TurnRight();
          motor1.run(MOTOR1_TUNE * moveSpeedTurn);
          motor2.run(MOTOR2_TUNE * moveSpeedTurn/10);
          }

        led.setColorAt(1, 255, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();

      // THE SILLY BEHAVIOR
      /* keep turning what it was turning
      if (turning_left) {
        motor1.run(MOTOR1_TUNE * 0);      //Left motor Stop
        motor2.run(MOTOR2_TUNE * 255.0);  //Right motor Run
      } else {
        motor1.run(MOTOR1_TUNE * 255.0);  //Left motor Run
        motor2.run(MOTOR2_TUNE * 0);      //Right motor Stop
      }*/
      break;
    default: break;
  } // End of 'switch' statement
  } // End of 'if' STOP_moving
}
