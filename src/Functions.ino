/*
* Function name: signOfNumber
* Parameters: float number
* Return: int
* Summary: gets the sign of a number
*/
int signOfNumber(float number)
{
  if (number < 0)
    return -1;
  else if (number >= 0)
    return 1;
}

/*
* Function name: mapPWM
* Parameters: float value, float fromLow, float fromHigh, float toLow, float toHigh
* Return: float
* Summary: maps a PWM signal to another scale given input ranges, output ranges, and the input value
*/
float mapPWM(float value, float fromLow, float fromHigh, float toLow, float toHigh)
{
  return (toLow + (toHigh - toLow) / (fromHigh - fromLow) * (value - fromLow));
}

/*
* Function name: rotationCalibration
* Parameters: void
* Return: void
* Summary: calibrates the rotation motor to the zero position using an end stop
*/
void rotationCalibration(void)
{
  // rotate to the endstop in the CCW direction
  while (!digitalRead(endstopPin1))
  {
     digitalWrite(dirPin1, LOW);
     digitalWrite(stepPin1, HIGH);
     delay(50);
     digitalWrite(stepPin1, LOW);
     delay(50);       
  }
  
  // rotate to the zero position in the CW direction
  for (int i = 0; i < abs(round(endstopPosition1)); i++)
  {
    digitalWrite(dirPin1, HIGH);
    digitalWrite(stepPin1, HIGH);
    delay(50);
    digitalWrite(stepPin1, LOW);
    delay(50);
  }
}

/*
* Function name: liftCalibration
* Parameters: void
* Return: void
* Summary: calibrates the lift motor to the zero position using an end stop
*/
void liftCalibration(void)
{
  // retract lift to endstop by rotating the spool in the CCW direction
  while (!digitalRead(endstopPin3))
  {
    digitalWrite(dirPin2, LOW);
    digitalWrite(stepPin2, HIGH);
    delay(50);
    digitalWrite(stepPin2, LOW);
    delay(50);
  }
}
