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
