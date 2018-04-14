void ExecuteTask(int sensorState) {
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
      
    default: break;
    }
}

