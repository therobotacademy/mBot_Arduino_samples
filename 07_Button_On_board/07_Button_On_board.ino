#include <MeMCore.h>

void setup()
{
  Serial.begin(9600);
}

boolean currentPressed = false;
boolean pre_buttonPressed = false;

void loop() {
  currentPressed = (analogRead(7) < 100);
  if (currentPressed != pre_buttonPressed)
  {
    pre_buttonPressed = currentPressed;
    if (currentPressed == true)
    {
      Serial.println("Button Down (Pressed)");
    } else
    {
      Serial.println("Button Up (Released)");
    }
  }
}