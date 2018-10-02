/*  Library for controlling 2 DC motors
*/

#include <TwoMotors.h>

TwoMotors car;

void setup()
{
  Serial.begin(9600);
  car.change_serial(Serial);
}

void loop()
{
  car.forward();
}
