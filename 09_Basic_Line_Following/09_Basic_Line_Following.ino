#include <MeMCore.h>

MeBuzzer buzzer;
MeLineFollower lineFinder(PORT_2);
MeDCMotor motor1(M1); //Motor1 is Left Motor
MeDCMotor motor2(M2); //Motor2 is Left Motor
MeRGBLed led(0, 30);

void setup() {
  led.setpin(13);
  pinMode(7, INPUT); //Define button pin as input
  while (analogRead(7) > 100) {
    delay(50); //Wait till button pressed to start.
  }
  buzzer.tone(200, 200);   //Buzzer beep to indicate start
}

float MOTOR1_TUNE = -1.0; //Left motor scale factor, negative for going forward
float MOTOR2_TUNE = 1.0;  //Right motor scale factor
float turning_left = true;

void loop() {
  int sensorState = lineFinder.readSensors();
  switch (sensorState)
  {
    case S1_IN_S2_IN:
      motor1.run(MOTOR1_TUNE * 255.0*0.5); //Left motor Run. Half of maximum speed to avoid both sensor out
      motor2.run(MOTOR2_TUNE * 255.0*0.5); //Right motor Run
      led.setColorAt(1, 0, 255, 0); //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 0, 255, 0); //Set LED0 (RGBLED1) (RightSide)
      led.show();
      break;
    case S1_IN_S2_OUT:
      //turn left
      motor1.run(MOTOR1_TUNE * 0);      //Left motor Stop
      motor2.run(MOTOR2_TUNE * 255.0);  //Right motor Run
      led.setColorAt(1, 0, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 0, 255, 0); //Set LED0 (RGBLED1) (RightSide)
      led.show();
      turning_left = true;
      break;
    case S1_OUT_S2_IN:
      //turn right
      motor1.run(MOTOR1_TUNE * 255.0);  //Left motor Run
      motor2.run(MOTOR2_TUNE * 0);      //Right motor Stop
      led.setColorAt(1, 0, 255, 0); //Set LED1 (RGBLED2) (LeftSide)
      led.setColorAt(0, 0, 0, 0);   //Set LED0 (RGBLED1) (RightSide)
      led.show();
      turning_left = false;
      break;
    case S1_OUT_S2_OUT:
      // Better testing alternative: stop moving
        motor1.run(MOTOR1_TUNE * 0);  //Left motor Stop
        motor2.run(MOTOR2_TUNE * 0);  //Right motor Stop
      //keep turning what it was turning
      /*if (turning_left) {
        motor1.run(MOTOR1_TUNE * 0);      //Left motor Stop
        motor2.run(MOTOR2_TUNE * 255.0);  //Right motor Run
        led.setColorAt(1, 0, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide)
        led.show();
      } else {
        motor1.run(MOTOR1_TUNE * 255.0);  //Left motor Run
        motor2.run(MOTOR2_TUNE * 0);      //Right motor Stop
        led.setColorAt(1, 255, 0, 0);   //Set LED1 (RGBLED2) (LeftSide)
        led.setColorAt(0, 0, 0, 0);     //Set LED0 (RGBLED1) (RightSide)
        led.show();
      }*/
      break;
    default: break;
  }
}
