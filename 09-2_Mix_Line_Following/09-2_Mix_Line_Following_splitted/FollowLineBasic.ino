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
