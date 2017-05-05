#include <Servo.h>


int val; //sets the command

Servo servoLeft;          // Define left servo
Servo servoRight;         // Define right servo

void setup() { 
  Serial.begin(115200);
  servoLeft.attach(D6);  // Set left servo to digital pin D6
  servoRight.attach(D7);  // Set right servo to digital pin D7

  
} 

void loop() {            // Put whatever you want to execute repeatedly here.
  
forward(5);
delay(5000);
  
}

// Motion routines for forward & right turn (about 45 degrees)
void forward(float time) {
  servoLeft.write(180);
  servoRight.write(0);
  delay(time*1000);
  val = 0;
  
}



void turnRight(float time) {
  servoLeft.write(180);
  servoRight.write(180);
  delay(time*1000);
  val = 0;
}

