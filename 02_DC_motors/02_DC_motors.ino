#include <MeMCore.h>

MeDCMotor motor1(M1); //Motor1 is Left Motor
MeDCMotor motor2(M2); //Motor2 is Right Motor

void setup(){}

void loop()
{
  //motor.run() maximum speed is 255 to -255, 0 is stop
  motor1.run(-100); //Motor1 (Left)  forward is -negative
  motor2.run(100);  //Motor2 (Right) forward is +positive
  delay(500);

  motor1.run(100);  //Motor1 (Left)  backward is +positive
  motor2.run(-100); //Motor2 (Right) backward is -negative
  delay(500);
  
  motor1.stop(); //Stop Motor1 
  motor2.stop(); //Stop Motor1 
  delay(500);
}