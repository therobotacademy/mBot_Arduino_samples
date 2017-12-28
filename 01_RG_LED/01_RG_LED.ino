#include <MeMCore.h>

MeRGBLed led(0, 30);

void setup()
{
  led.setpin(LED_BUILTIN); // As in Arduino UNO, corresponds to pin 13
}

void loop()
{
  led.setColor(255, 255, 255); //Set both LED to White
  led.show();                  //Must use .show() to make new colour take effect.
  delay(500);

  led.setColorAt(0, 255, 0, 0); //Set LED0 (RGBLED1) (RightSide) to Red
  led.setColorAt(1, 0, 0, 255); //Set LED1 (RGBLED2) (LeftSide)  to Blue
  led.show();
  delay(500);

  led.setColorAt(0, 0, 0, 255); //Set LED0 (RGBLED1) (RightSide) to Blue
  led.setColorAt(1, 255, 0, 0); //Set LED1 (RGBLED2) (LeftSide)  to Red
  led.show();
  delay(500);
}

