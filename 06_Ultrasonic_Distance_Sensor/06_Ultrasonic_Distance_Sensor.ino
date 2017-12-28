#include <MeMCore.h>

MeUltrasonicSensor ultrasonic(PORT_3);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("distance(cm) = ");          // Print the results to the serial monitor
  Serial.println(ultrasonic.distanceCm());  // Distance value from 3cm - 400cm
  delay(50);                                // Wait 50 milliseconds before next measurement
}
