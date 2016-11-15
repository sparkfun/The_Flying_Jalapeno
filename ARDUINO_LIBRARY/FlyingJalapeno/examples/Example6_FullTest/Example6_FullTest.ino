/*
  This example shows how to run a normal test procedure

  Pete Lewis, started on 11/3/2016
  Contributions by NES November 15th, 2016

  Select Mega2560 from the boards list
*/
#define STATUS_LED 13

#include <FlyingJalapeno.h>
FlyingJalapeno FJ(STATUS_LED); //Blink status msgs on pin 13

#include <CapacitiveSensor.h>
CapacitiveSensor cs_1 = CapacitiveSensor(47, 45);
CapacitiveSensor cs_2 = CapacitiveSensor(31, 46);

int failures = 0; //Number of failures by the main test routine

boolean targetPowered = false; //Keeps track of whether power supplies are energized

long preTestButton = 0; //Cap sense values for two main test buttons
long testButton = 0;

void setup()
{
  pinMode(STATUS_LED, OUTPUT);

  FJ.enablePCA(); //Enable the I2C buffer

  Serial.begin(9600);
  Serial.println("Testbed\n\r");
}

void loop()
{
  preTestButton = cs_1.capacitiveSensor(30);
  testButton = cs_2.capacitiveSensor(30);

  //Serial.print(preTestButton);
  //Serial.print("\t");
  //Serial.println(testButton);

  //Is user pressing PreTest button?
  if (preTestButton > 5000)
  {
    FJ.dot(); //Blink status LED to indicate button press

    if (targetPowered == true) 
    {
      power_down(); //Power down the test jig
    }
    else
    {
      //Check both power supplies for shorts to ground
      if (FJ.powerTest(1) == true && FJ.powerTest(2) == true)
      {
        //FJ.setV2(true, 4.2); // charge led off
        //delay(2000);

        FJ.setV1(true, 5); //Turn on power supply 1 to 5.0V
        
        delay(500);

        Serial.println("Pre-test PASS, powering up...\n\r");

        targetPowered = true;

        digitalWrite(LED_PT_PASS, HIGH);
        digitalWrite(LED_PT_FAIL, LOW);

        delay(500); // debounce touching
      }
      else
      {
        //Power supply test failed
        failures++;

        FJ.setV1(false, 5); //Turn off power supply 1
        
        Serial.println("Jumper on Power Rail V1\n\r");
        
        targetPowered = false;
        
        digitalWrite(LED_PT_FAIL, HIGH);
        digitalWrite(LED_PT_PASS, LOW);

        delay(500); // debounce touching
      }
    }
  }
  else if (testButton > 5000 && targetPowered == true)
  {
    //Begin main test
    
    FJ.dot();

    failures = 0; // reset for testing a second time

    digitalWrite(LED_PASS, LOW);
    digitalWrite(LED_FAIL, LOW);

    //test_33V();
    test(); //Run main test code

    if (failures == 0)
    {
      digitalWrite(LED_PASS, HIGH);
      charge_led_blink();
    }
    else
    {
      digitalWrite(LED_FAIL, HIGH);
    }
  }
}

void test()
{
  // add in your test code here
}

// This is an example of testing a 3.3V output from the board sent to A2.
// This was originally used on the ESP32 dev board testbed.
void test_33V()
{
  Serial.println("testing 3.3V output on board");

  //pin = pin to test
  //correct_val = what we expect.
  //allowance_percent = allowed window for overage. 0.1 = 10%
  //debug = print debug statements
  boolean result = FJ.verify_voltage(A2, 516, 0.1, true); // 3.3V split by two 10Ks, reads 516 on my 3.3V logic FJ (using a proto known good).

  if (result == true) 
    Serial.println("test success!");
  else
  {
    Serial.println("test failure (should read near 516)");
    failures++;
  }
}

void power_down()
{
  Serial.println("powering down target");

  FJ.setV1(false, 5); //Turn off power supply 1, but leave voltage selection at 5V
  FJ.setV2(false, 4.2); //Turn off power supply 1, but leave voltage selection at 4.2V

  targetPowered = false;

  //Turn off all LEDs
  digitalWrite(LED_PT_PASS, LOW);
  digitalWrite(LED_PT_FAIL, LOW);
  digitalWrite(LED_PASS, LOW);
  digitalWrite(LED_FAIL, LOW);

  failures = 0;

  delay(500); // debounce touching
}

void charge_led_blink()
{
  for (int i = 0 ; i < 3 ; i++)
  {
    FJ.setV2(true, 3.7); // charge led ON
    delay(500);
    FJ.setV2(true, 4.2); // charge led off
    delay(500);
  }
}
