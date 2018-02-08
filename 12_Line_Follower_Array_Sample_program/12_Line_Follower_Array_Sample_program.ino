
const int DataPin = 12;
uint8_t Sensor_Data[3];

void setup() 
{
    Serial.begin(9600);
    delay(100);
    Serial.println("Start.....");
}

void loop()
{
    Serial.println(getValue(),BIN);
    delay(100);
}

uint8_t getValue()
{  
    long time_out_flag = 0;
    pinMode(DataPin, OUTPUT);
    digitalWrite(DataPin, LOW);
    delayMicroseconds(980);
    digitalWrite(DataPin, HIGH);
    delayMicroseconds(40);
    pinMode(DataPin, INPUT_PULLUP);
    delayMicroseconds(50); 
    time_out_flag = millis();
    while((digitalRead(DataPin) == 0)&&((millis() - time_out_flag) < 6)); 
    time_out_flag = millis();
    while((digitalRead(DataPin) == 1)&&((millis() - time_out_flag) < 6));
    for(uint8_t k=0; k<3; k++)
    {
        Sensor_Data[k] = 0x00;
        for(uint8_t i=0;i<8;i++)
        {
            time_out_flag = millis(); 
            while(digitalRead(DataPin) == 0&&((millis() - time_out_flag) < 6));
            uint32_t HIGH_level_read_time = micros();
            time_out_flag = millis(); 
            while(digitalRead(DataPin) == 1&&((millis() - time_out_flag) < 6));
            HIGH_level_read_time = micros() - HIGH_level_read_time;
           if(HIGH_level_read_time > 50 && HIGH_level_read_time < 100)  
            {
                Sensor_Data[k] |= (0x80 >> i);
            }
        }
    }
    if (Sensor_Data[1] == (uint8_t)(~(uint8_t)Sensor_Data[0]))
    {
       return Sensor_Data[0];
    }
}
