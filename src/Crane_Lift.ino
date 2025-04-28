// libraries
#include <PinChangeInterrupt.h>

// PWM range values for the receiver
float maxPWM = 1900;
float minPWM = 1100;
float center = 1500;
float deadzone = 50;

// receiver channels
const int channelPins[4] = {2, 3, 4, 5};
const int channelInverseOnOff[4] = {false, true, true, true};
float channelPWM[4] = {1000, 1500, 1500, 1500};  // throttle, pitch, roll, yaw, VTOL switch

// receiver arrays
volatile uint32_t receiverNow[6], receiverPast[6], receiverInput[6], receiverUpdateTime[6];

// stepper motor pins
const int dirPin1 = 9;
const int stepPin1 = 10; // LOW - CCW, HIGH - CW
const int dirPin2 = 11;
const int stepPin2 = 12; // LOW - CCW, HIGH - CW
long unsigned motorTimer1;
long unsigned motorTimer2;

// motor control
float motorInterval = 5000;
float motorFastInterval1 = 500; 
float motorSlowInterval1 = 5000;
float motorFastInterval2 = 500;
float motorSlowInterval2 = 5000;
bool motorSwitch1 = true;
bool motorSwitch2 = true;
float stepsPerRotation = 400; // number of steps per full rotation (200 - full steps, 400 - half steps)
float stepSize = 360 / stepsPerRotation;
float motorPosition1 = 0; // position of motor 1 in degrees
float motorPosition2 = 0; // position of motor 2 in degrees

// endstop pins
const int endstopPin1 = 6;
const int endstopPin2 = 7;
const int endstopPin3 = 8;

// endstop position in degrees
float endstopPosition1 = -90; // CCW side for rotation
float endstopPosition2 = 90; // CW side for rotation
float endstopPosition3 = 0;

void setup() 
{
  Serial.begin(115200);
  receiverInitialize();
  
  // intialize endstop pins as inputs
  pinMode(endstopPin1, INPUT);
  pinMode(endstopPin2, INPUT);
  pinMode(endstopPin3, INPUT);
    
  // initialize stepper motor direction and step pins as outputs
  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  motorTimer1 = motorTimer2 = micros();
  
  delay(5000);
  
  // calibrate the stepper motor positions
  liftCalibration();
  rotationCalibration();
  
  delay(1000);
}

void loop() 
{  
  // *************** CRANE ROTATION MOTOR *************** 
  float magnitude1 = (abs(channelPWM[2] - center)); // get the magnitude offset of the PWM signal from the midpoint (1500)
  int direction1 = signOfNumber(channelPWM[2] - center); // get the direction the motor should spin (signal - midpoint)
    
  // (magnitude1 > deadzone) checks if the joystick is outside of the deadzone
  // (micros() - motorTimer1 >= motorInterval)) waits to move motor only if the specified time interval has passed by
  // (motorSwitch1 == false) checks if the write pin to the motor needs to be turned to LOW
  if (((magnitude1 > deadzone) && (micros() - motorTimer1 >= motorInterval)) || ((motorSwitch1 == false) && (micros() - motorTimer1 >= motorInterval)))
  {
    motorInterval = mapPWM(magnitude1, deadzone, maxPWM - center, motorSlowInterval1, motorFastInterval1);
    // set stepper motor direction
    if (direction1 == 1)
      digitalWrite(dirPin1, HIGH);
    else if (direction1 == -1)
      digitalWrite(dirPin1, LOW);
    
    // sets the position of the motor to that of the endstops if the endstops are pressed
    if (digitalRead(endstopPin1))
      motorPosition1 = endstopPosition1;
    else if (digitalRead(endstopPin2))
      motorPosition1 = endstopPosition2;
      
    // (motorSwitch1) checks if the motor write pin should be set to HIGH
    // !(direction1 == -1 && digitalRead(endstopPin1)) checks that the endstop and the direction are not the same
    if (motorSwitch1 && !(direction1 == -1 && digitalRead(endstopPin1)) && !(direction1 == 1 && digitalRead(endstopPin2)))
    {
      // step motor
      digitalWrite(stepPin1, HIGH);
      motorSwitch1 = false;
            
      // updates the position of the motor
      if (direction1 = 1)
        motorPosition1 += stepSize;
      else if (direction1 = -1)
        motorPosition1 -= stepSize;
    }
    
    // (!motorSwitch1) checks if the motor write pin should be set to LOW)
    else if (!motorSwitch1)
    {
      // disable motor
      digitalWrite(stepPin1, LOW);
      motorSwitch1 = true;
    }
    motorTimer1 = micros();
  }

    
  // ***************** CRANE LIFT MOTOR ***************** 
  float magnitude2 = (abs(channelPWM[3] - center)); // get the magnitude offset of the PWM signal from the midpoint (1500)
  int direction2 = signOfNumber(channelPWM[3] - center); // get the direction the motor should spin (signal - midpoint)
    
  // (magnitude2 > deadzone) checks if the joystick is outside of the deadzone
  // (micros() - motorTimer2 >= motorInterval)) waits to move motor only if the specified time interval has passed by
  // (motorSwitch2 == false) checks if the write pin to the motor needs to be turned to LOW
  if (((magnitude2 > deadzone) && (micros() - motorTimer2 >= motorInterval)) || ((motorSwitch2 == false) && (micros() - motorTimer2 >= motorInterval)))
  {
    motorInterval = mapPWM(magnitude2, deadzone, maxPWM - center, motorSlowInterval2, motorFastInterval2);
    // set stepper motor direction
    if (direction2 == 1)
      digitalWrite(dirPin2, HIGH);
    else if (direction1 == -1)
      digitalWrite(dirPin2, LOW);
        
    // sets the position of the motor to the position of the endstop if the endstop is pressed
    if (digitalRead(endstopPin3))
      motorPosition2 = endstopPosition3;
    
    // (motorSwitch2) checks if the motor write pin should be set to HIGH
    // !(direction1 == 1 && digitalRead(endstopPin3)) checks that the endstop and the direction are not the same
    if (motorSwitch2 && !(direction1 == 1 && digitalRead(endstopPin3)))
    {
      // step motor          
      digitalWrite(stepPin2, HIGH);
      motorSwitch2 = false;
            
      // updates the position of the motor
      if (direction2 = 1)
        motorPosition2 += stepSize;
      else if (direction2 = -1)
        motorPosition2 -= stepSize;
    }
    
    // (!motorSwitch1) checks if the motor write pin should be set to LOW)
    else if (!motorSwitch2)
    {
      // disable motor
      digitalWrite(stepPin2, LOW);
      motorSwitch2 = true;
    }
    motorTimer2 = micros();
  }
}
