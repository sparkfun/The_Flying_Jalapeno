#include <FlyingJalapeno.h>
FlyingJalapeno FJ(13);

#include <CapacitiveSensor.h>
CapacitiveSensor   cs_4_2 = CapacitiveSensor(4,2);

void setup()
{
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
}

void loop()
{
  long total1 =  cs_4_2.capacitiveSensor(30);
  FJ.dot(); FJ.dot(); FJ.dot();

  delay(10);
  boolean test_result = FJ.verify_voltage(A6, 613, 0.1); // send this function pin, correct value, and a "percent" variance allowance - should be 0-1 - i.e. 10% variance would be "0.1"
  Serial.print(test_result);
  delay(10);
}