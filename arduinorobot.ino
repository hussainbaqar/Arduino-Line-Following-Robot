#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

//MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6

#define LINE_SEN_1 A2  // Renamed for clarity
#define LINE_SEN_2 A3

#define EN_PIN_1 13
#define EN_PIN_2 12

#define MOTOR_1 0
#define MOTOR_2 1

#define LINE_THRESHOLD 500  // Adjust based on your specific sensors and lighting conditions

short usSpeed = 40;  //default motor speed
unsigned short usMotor_Status = BRAKE;
 
void setup() {
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(MOTOR_A2_PIN, OUTPUT);
  pinMode(MOTOR_B2_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(PWM_MOTOR_2, OUTPUT);

  // Correctly configure sensor pins as input
  pinMode(LINE_SEN_1, INPUT);
  pinMode(LINE_SEN_2, INPUT);  

  pinMode(EN_PIN_1, INPUT);
  pinMode(EN_PIN_2, INPUT);
  digitalWrite(EN_PIN_1, HIGH);
  digitalWrite(EN_PIN_2, HIGH); 

  Serial.begin(9600);
  Serial.println("Begin line following");
}

void loop() {
  int leftSensor = digitalRead(LINE_SEN_1);
  int rightSensor = digitalRead(LINE_SEN_2);

  // Print sensor readings for debugging
  Serial.print("Left Sensor: ");
  Serial.print(leftSensor);
  Serial.print(" | Right Sensor: ");
  Serial.println(rightSensor);

  // Detect if the robot is moving out of track
  DetectOutOfTrack(leftSensor, rightSensor);

  // Motor control logic based on line sensors
  if (leftSensor < LINE_THRESHOLD && rightSensor < LINE_THRESHOLD) {
    // Both sensors detect the line, move forward
    Forward();
  } else if (leftSensor < LINE_THRESHOLD) {
    // Only left sensor detects the line, turn left
    TurnLeft();
  } else if (rightSensor < LINE_THRESHOLD) {
    // Only right sensor detects the line, turn right
    TurnRight();
  } else {
    // No sensor detects the line, stop
    Stop();
  }

}

// Function to detect if the robot is moving out of track
void DetectOutOfTrack(int leftSensor, int rightSensor) {
  if (leftSensor > LINE_THRESHOLD && rightSensor > LINE_THRESHOLD) {
    // Both sensors detect that the robot is out of track
    // Turn off one of the motors based on which sensor is triggered
    // If left sensor detects, turn off motor 1 (left motor)
    digitalWrite(EN_PIN_1, LOW);
    // If right sensor detects, turn off motor 2 (right motor)
    digitalWrite(EN_PIN_2, LOW);
    Serial.println("Robot is out of track! Turning off one motor.");
  } else {
    // If both sensors detect the line or are below threshold, enable both motors
    digitalWrite(EN_PIN_1, HIGH);
    digitalWrite(EN_PIN_2, HIGH);
  }
}
// Keep the rest of the functions (TurnLeft, TurnRight, Stop, Forward, Reverse, IncreaseSpeed, DecreaseSpeed, motorGo) as previously defined

void TurnLeft() {
  // Implement turning left by adjusting motor speeds or directions
  // Example: For a differential drive, stop or slow down the left motor and speed up the right motor
  Serial.println("Turn Left");
  motorGo(MOTOR_1, CCW, usSpeed / 2); // Example: slow down or reverse left motor
  motorGo(MOTOR_2, CW, usSpeed); // Speed up right motor
}

void TurnRight() {
  // Implement turning right
  Serial.println("Turn Right");
  motorGo(MOTOR_1, CW, usSpeed); // Speed up left motor
  motorGo(MOTOR_2, CCW, usSpeed / 2); // Example: slow down or reverse right motor
}


void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
  motorGo(MOTOR_2, usMotor_Status, 0);
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  motorGo(MOTOR_2, usMotor_Status, usSpeed);  
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_1, pwm); 
  }
  else if(motor == MOTOR_2)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A2_PIN, HIGH);
      digitalWrite(MOTOR_B2_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A2_PIN, LOW);
      digitalWrite(MOTOR_B2_PIN, LOW);            
    }
    
    analogWrite(PWM_MOTOR_2, pwm);
  }
}


