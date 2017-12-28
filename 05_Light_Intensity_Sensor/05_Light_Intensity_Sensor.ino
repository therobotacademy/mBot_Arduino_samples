#include <MeMCore.h>

MeLightSensor lightSensor(PORT_8);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print("value = ");           // Print the results to the serial monitor
  Serial.println(lightSensor.read()); // Brightness value from 0-1023
  delay(50);                          // Wait 50 milliseconds before next measurement
}