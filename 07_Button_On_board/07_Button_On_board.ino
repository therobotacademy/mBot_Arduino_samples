#include <MeMCore.h>
#define ON_BOARD_BUTTON A7
#define READ_THRESHOLD 100

void setup()
{
  Serial.begin(9600);
}

boolean currentPressed = false;
boolean pre_buttonPressed = false;

void loop() {
  currentPressed = (analogRead(ON_BOARD_BUTTON) < READ_THRESHOLD);
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
