// From document:
//	"Building Autonomous Line Followers using Arduino and PID.pdf"

long sensors_average;
int sensors_sum;
int position;
long sensors[] = {0, 0, 0, 0, 0}; // Array used to store 5 readings for 5 sensors

void setup()
{
	Serial.begin(9600);
}

/* Calculate and display robot lateral position
void loop()
{
	sensors_average = 0;
	sensors_sum = 0;
	for (int i = 0; i < 5; i++)
	{
		sensors[i] = analogRead(i);
		sensors_average += sensors[i] * i * 1000; //Calculating the weighted mean
		sensors_sum += int(sensors[i]);} //Calculating sum of sensor readings
		position = int(sensors_average / sensors_sum);
		Serial.print(sensors_average);
		Serial.print(' ');
		Serial.print(sensors_sum);
		Serial.print(' ');
		Serial.print(position);
		Serial.println();
		delay(2000);
	}
}
*/
	
/* Short version without Serial.print
	void loop()
{
	sensors_average = 0;
	sensors_sum = 0;
	for (int i = 0; i < 5; i++)
	{
		sensors[i] = analogRead(i);
		sensors_average += sensors[i] * i * 1000; //Calculating the weighted mean of the sensor readings
		sensors_sum += int(sensors[i]); //Calculating sum of sensor readings
		position = int(sensors_average / sensors_sum);
	}
}
*/

void loop()
{
	sensors_read(); //Reads sensor values and computes sensor sum and weighted average
	pid_calc(); //Calculates position[set point] and computes Kp,Ki and Kd
	calc_turn(); //Computes the error to be corrected
	motor_drive(right_speed, left_speed); //Sends PWM signals to the motors
}
	
void pid_calc()
{
	position = int(sensors_average / sensors_sum);
	proportional = position – set_point; // Replace set_point by your set point
	integral = integral + proportional;
	derivative = proportional - last_proportional;
	last_proportional = proportional;
	error_value = int(proportional * Kp + integral * Ki + derivative * Kd);
}

void alc_turn()
{ //Restricting the error value between +256.
	if (error_value< -256)
		error_value = -256;
	if (error_value> 256)
		error_value = 256;

// If error_value is less than zero calculate right turn speed values
	if (error_value< 0)
		{
		right_speed = max_speed + error_value;
		left_speed = max_speed;
		}
	// If error_value is greater than zero calculate left turn values
	else
		{
		right_speed = max_speed;
		left_speed = max_speed - error_value;
		}
}

void motor_drive(intright_speed, intleft_speed)
	{ // Drive motors according to the calculated values for a turn
		analogWrite(motor_right, right_speed);
		analogWrite(motor_left, left_speed);
		delay(50); // Optional
	}
	
