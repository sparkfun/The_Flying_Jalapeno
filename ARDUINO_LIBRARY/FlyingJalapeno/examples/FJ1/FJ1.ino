#include <FlyingJalapeno.h>

FlyingJalapeno FJ(13);

void setup()
{
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

void loop()
{
  FJ.dot(); FJ.dot(); FJ.dot();

  delay(10);
  boolean test_result = FJ.verify_voltage(A6, 613, 0.1); // send this function pin, correct value, and a "percent" variance allowance - should be 0-1 - i.e. 10% variance would be "0.1"
  Serial.print(test_result);
  delay(10);
}