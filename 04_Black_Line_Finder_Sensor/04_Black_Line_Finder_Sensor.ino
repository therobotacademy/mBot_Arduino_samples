#include <MeMCore.h>

MeLineFollower lineFinder(PORT_2); 

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  int sensorState = lineFinder.readSensors();
  switch(sensorState)
  {
    case S1_IN_S2_IN:   Serial.println("S1_IN(izq NEGRO)  S2_IN(dcho NEGRO)"); break;
    case S1_IN_S2_OUT:  Serial.println("S1_IN(izq NEGRO)  S2_OUT(dcho BLANCO)"); break;
    case S1_OUT_S2_IN:  Serial.println("S1_OUT(izq BLANCO)  S2_IN(dcho NEGRO)"); break;
    case S1_OUT_S2_OUT: Serial.println("S1_OUT(izq BLANCO)  S2_OUT(dcho BLANCO)"); break;
    default: break;
  }
  delay(200);
}
