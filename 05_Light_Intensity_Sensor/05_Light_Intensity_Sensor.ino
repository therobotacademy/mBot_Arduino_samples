#include <MeMCore.h>

MeLightSensor lightSensor(PORT_8);
MeRGBLed led(0, 30);  // Needed to act on RGB LEDs to ensure they do not affect the light sensor reading

void setup()
{
  Serial.begin(9600);
  led.setpin(LED_BUILTIN); // As in Arduino UNO, corresponds to pin 13
  led.setColor(0, 0, 0); //Set both LED to Black -> OFF
  led.show();            //Must use .show() to make new colour take effect.
}

void loop()
{
  Serial.print("value = ");           // Print the results to the serial monitor
  Serial.println(lightSensor.read()); // Brightness value from 0-1023 (10 bits analog pin output)
  delay(50);                          // Wait 50 milliseconds before next measurement
}
