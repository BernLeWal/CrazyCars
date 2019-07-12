/*
 * TestServo
 * 
 * Test the Servo-Controls within the CrazyCars Prototype Board
 * @author: Bernhard Wallisch
 * 
 * Servo-Test sample is derived from https://funduino.de/nr-12-servo-ansteuern
 * Changes to the original wiring see TestServo_Wiring.png: 
 *  - connect the first servo to pin 10 (instead of 8)
 *  - connect the second servo to pin 11
 */

#include <Servo.h> 

Servo servo1; // will be for the motor
Servo servo2; // will be for the steering-servo

void setup()
{
  servo1.attach(10); 
  servo2.attach(11);
}

void loop()
{ 
  servo1.write(90); // motor stop
  servo2.write(90); // steering in center direction
  delay(3000); 
  servo1.write(0); // car drives backwards
  servo2.write(0); // steering to the left maximum
  delay(3000); 
  servo1.write(180); // car drives forward
  servo2.write(180); // steering to the right maximum
  delay(3000);
}
