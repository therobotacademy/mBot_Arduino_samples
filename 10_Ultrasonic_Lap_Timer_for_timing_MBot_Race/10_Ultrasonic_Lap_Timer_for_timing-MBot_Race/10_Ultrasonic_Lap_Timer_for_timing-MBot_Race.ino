#include <MeMCore.h>

MeBuzzer buzzer;
const int buzzerDuration = 20;

MeUltrasonicSensor ultrasonic(PORT_3);
const float distanceThreshold = 10.0;
float distance = 10.0;
boolean detected = false;
unsigned int detectCount = 0;

unsigned long currentTime = 0;
unsigned long firstDetectMills = 0;
unsigned long lastDetectMills = 0;
const float minimumLapTime = 1000; //ms

void setup()
{
  Serial.begin(115200);
  Serial.println("Lap Timer.");
  Serial.println("Trigger the sensor to start timing");
  Serial.print("Sensor detection distance: ");
  Serial.print(distance);
  Serial.println("cm");
  buzzer.tone(600, buzzerDuration);   //Buzzer sounds 600Hz for 1000ms
  delay(100);
  buzzer.tone(600, buzzerDuration);   //Buzzer sounds 600Hz for 1000ms
  delay(100);
  buzzer.tone(600, buzzerDuration);   //Buzzer sounds 600Hz for 1000ms
  delay(100);
  buzzer.tone(900, buzzerDuration * 2); //Buzzer sounds 600Hz for 1000ms
}

void loop()
{
  currentTime = millis();
  if ((currentTime - lastDetectMills) > minimumLapTime) {
    distance = ultrasonic.distanceCm();
    if ((distance < distanceThreshold)) {
      if (!detected) {
        detected = true;
        if (detectCount == 0) {
          firstDetectMills = currentTime;
          buzzer.tone(300, buzzerDuration);   //Buzzer sounds 600Hz for 1000ms
        }
        Serial.print("Lap:");
        Serial.print(detectCount);
        Serial.print(",  Time:");
        Serial.print(currentTime - firstDetectMills);
        Serial.print("ms,  LapTime:");
        Serial.print(currentTime - lastDetectMills);
        Serial.print(" ms, UltrasoundDistance:");
        Serial.print(distance);
        Serial.println(" cm");
        buzzer.tone(600, buzzerDuration);   //Buzzer sounds 600Hz for 1000ms
        detectCount++;
        lastDetectMills = currentTime;
      }
    } else {
      detected = false;
    }//distance < distanceThreshold
  }//minimumLapTime
}//loop