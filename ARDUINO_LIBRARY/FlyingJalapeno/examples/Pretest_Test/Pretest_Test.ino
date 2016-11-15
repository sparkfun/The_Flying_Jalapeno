// Pete Lewis, started on 11/3/2016
// Example code for a testbed using the Flying Jalapeno (mega2560 based dev board)

#define ERROR_LED 13

#include <FlyingJalapeno.h>
FlyingJalapeno FJ(ERROR_LED); //Blink error msgs on pin 13

#include <CapacitiveSensor.h>
CapacitiveSensor cs_1 = CapacitiveSensor(47, 45);
CapacitiveSensor cs_2 = CapacitiveSensor(31, 46);

int failures = 0;
boolean targetPower = false;
long total1 = 0;
long total2 = 0;

void setup()
{
  pinMode(ERROR_LED, OUTPUT);

  FJ.PCA_enable(true);

  Serial.begin(9600);
  Serial.println("Testbed\n\r");

  //FJ.setV2(true, 4.2); // charge led off
}

void loop()
{
  total1 = cs_1.capacitiveSensor(30);
  total2 = cs_2.capacitiveSensor(30);

  //Serial.print(total1);
  //Serial.print("\t");
  //Serial.println(total2);

  if (total1 > 5000)
  {
    FJ.dot();

    if (target_power) power_down();
    else
    {
      //Check both power supplies for shorts to ground
      if (FJ.powerTest(1) == true && FJ.powerTest(2) == true)
      {
        //FJ.setV2(true, 4.2); // charge led off
        //delay(2000);

        FJ.setV1(true, 5); //Turn on power supply
        
        delay(500);

        Serial.println("Pre-test PASS, powering up...\n\r");

        target_power = true;

        digitalWrite(LED_PT_PASS, HIGH);
        digitalWrite(LED_PT_FAIL, LOW);

        delay(500); // debounce touching
      }
      else
      {
        //Power supply test failed
        failures++;

        FJ.setV1(false, 5);
        
        Serial.println("Jumper on Power Rail V1\n\r");
        
        target_power = false;
        
        digitalWrite(LED_PT_FAIL, HIGH);
        digitalWrite(LED_PT_PASS, LOW);
        delay(500); // debounce touching
      }
    }
  }
  else if ((total2 > 5000) && (target_power))
  {
    FJ.dot();
    failures = 0; // reset for testing a second time
    digitalWrite(LED_PASS, LOW);
    digitalWrite(LED_FAIL, LOW);
    //test_33V();
    if (failures == 0) test();
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
  boolean result;
  result = FJ.verify_voltage(A2, 516, 0.1, true); // 3.3V split by two 10Ks, reads 516 on my 3.3V logic FJ (using a proto known good).
  if (result == true) Serial.println("test success!");
  else
  {
    Serial.println("test failure (should read near 516)");
    failures++;
  }
}

void power_down()
{
  Serial.println("powering down target");
  FJ.setV1(false, 5); // if power is already on, then just turn it off.
  FJ.setV2(false, 4.2); // charge fakeout off
  target_power = false;
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
