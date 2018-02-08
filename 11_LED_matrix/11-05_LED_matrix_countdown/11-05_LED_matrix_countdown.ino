// include mCore Makeblock library
#include <MeMCore.h>  // mCore Makeblock library

MeLEDMatrix Matrix_1(PORT_4);
MeRGBLed led(0, 30); // RGB LEDs
/* led.setColorAt(0/1, 255, 0, 0); // LED0 (RGBLED1) (RightSide), LED1 (RGBLED2) (LeftSide)
 *  // 0-255 for each of 3 positions: Red, Green, Blue
 *  led.show(); // Make the LED on
 */
 
int number = 10;
int count_max = number;
String leido = "";

void setup()
{
  Serial.begin(9600);
  Serial.println("Número de inicio de cuenta atrás: ");
  while (Serial.available() == 0) {
    //No hago nada
  }
  do {
    char caracter_leido;
    delay(5);
    caracter_leido = Serial.read();
    if (1){
    Serial.print("Caracter leido: ");
    Serial.print(caracter_leido);
    Serial.print(" --> ");
    Serial.println(caracter_leido, DEC);
    Serial.print("Caracteres en buffer: ");
    Serial.println(Serial.available());
    Serial.println();
  }
    leido += caracter_leido;
  }  while (Serial.available() > 0);

  count_max = leido.toInt();
  number = count_max; // First value to show in LED matrix
  Serial.print("");

  Serial.print("The countdown starts from... ");
  Serial.println(count_max);
//  Serial.println("The countdown starts from \"" + data + "\"");
   
  Matrix_1.setBrightness(Brightness_8);

  led.setpin(LED_BUILTIN); // As in Arduino UNO, corresponds to pin 13
  led.setColorAt(0, 255,0,0); // Stay RED when exceeded to negative numbers (below zero)
  led.setColorAt(1, 255,0,0);
}
void loop()
{
  if (number >= -count_max) {
    // Conver number to char array in order to can show it in the LED matrix
    char data[1];
    itoa(number,data,10);
    Serial.println(data);
    Matrix_1.drawStr(0,7,data);

    //And blink the RGB LEDs
    led.setColorAt(0, 255,255,255); // WHITE to correspond wit HIGH
    led.setColorAt(1, 255,255,255);
    led.show(); delay(900);
    led.setColorAt(0, 0,0,255); // BLUE to correspond wit LOW
    led.setColorAt(1, 0,0,255);
    led.show(); delay(100);
    number--;
    }
  led.setColorAt(0, 255,0,0); // Stay RED when exceeded to negative numbers (below zero)
  led.setColorAt(1, 255,0,0);
  led.show();
}

