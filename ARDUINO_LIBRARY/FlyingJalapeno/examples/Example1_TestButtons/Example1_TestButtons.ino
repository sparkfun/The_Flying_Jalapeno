/*
  This example shows how to read the two capacitive touch buttons on every test jig.

  Pete Lewis, started on 11/3/2016
  Contributions by NES November 15th, 2016

  Select Mega2560 from the boards list
*/

#define STATUS_LED 13

#include <FlyingJalapeno.h>
FlyingJalapeno FJ(STATUS_LED); //Blink status msgs on pin 13

#include <CapacitiveSensor.h>
CapacitiveSensor button1 = CapacitiveSensor(47, 45); //Wired to pins 47/45 on nearly every jig
CapacitiveSensor button2 = CapacitiveSensor(31, 46); //Wired to pins 31/46 on nearly every jig

long preTestButton = 0; //Cap sense values for two main test buttons
long testButton = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("Testbed\n\r");

  digitalWrite(LED_PRETEST_PASS, LOW);
  digitalWrite(LED_PRETEST_FAIL, LOW);
  digitalWrite(LED_PASS, LOW);
  digitalWrite(LED_FAIL, LOW);
}

void loop()
{
  preTestButton = button1.capacitiveSensor(30);
  testButton = button2.capacitiveSensor(30);

  //Is user pressing PreTest button?
  if (preTestButton > 5000)
  {
    Serial.println("You press pretest!");
    
    digitalWrite(LED_PT_PASS, HIGH);
    digitalWrite(LED_PT_FAIL, LOW);

    delay(500); // debounce touching
  }
  else if (testButton > 5000)
  {
    Serial.println("You press test!");
    
    digitalWrite(LED_PASS, HIGH);
    digitalWrite(LED_FAIL, LOW);

    delay(500); // debounce touching
  }
}

