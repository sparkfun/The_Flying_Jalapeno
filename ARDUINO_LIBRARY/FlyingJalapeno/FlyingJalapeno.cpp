/*
  FlyingJalapeno.cpp - Library for SparkFun Qc's general testbed board, The Flying Jalapeno
  Created by Pete Lewis, June 29th, 2015
  Released into the public domain.
*/

#include "Arduino.h"
#include "FlyingJalapeno.h"

//Given a pin
FlyingJalapeno::FlyingJalapeno(int statLED)
{
  _statLED = statLED;

  pinMode(_statLED, OUTPUT);

  pinMode(LED_PT_PASS, OUTPUT);
  pinMode(LED_PT_FAIL, OUTPUT);
  pinMode(LED_PASS, OUTPUT);
  pinMode(LED_FAIL, OUTPUT);

}

void FlyingJalapeno::dot()
{
  digitalWrite(_statLED, HIGH);
  delay(250);
  digitalWrite(_statLED, LOW);
  delay(250);
}

void FlyingJalapeno::dash()
{
  digitalWrite(_statLED, HIGH);
  delay(1000);
  digitalWrite(_statLED, LOW);
  delay(250);
}

// PRE-TEST for shorts to GND on power rails, returns true if all is good, returns false if there is short detected
boolean FlyingJalapeno::powerTest(byte select) // select is for either "1" or "2" for using either pretest resistors on the FJ
{
  byte control_pin = 44;

  byte read_pin;
  if (select == 1) read_pin = A14;
  else if (select == 2) read_pin = A15;

  pinMode(control_pin, OUTPUT);
  pinMode(read_pin, INPUT);

  digitalWrite(control_pin, HIGH);
  delay(200);
  int reading = analogRead(read_pin);

  Serial.print("Jumper test reading:");
  Serial.println(reading);

  digitalWrite(control_pin, LOW);
  pinMode(control_pin, INPUT);

  int jumper_val = 486;

  if ((reading < (jumper_val * 1.03)) && (reading > (jumper_val * 0.97))) return false; // jumper detected!!
  else return true;
}

//Test a pin to see what voltage is on the pin.
//Returns true if pin voltage is within a given window of the value we are looking for
//pin = pin to test
//correct_val = what we expect. If 
//allowance_percent = allowed window for overage. 0.1 = 10%
//debug = print debug statements
boolean FlyingJalapeno::verify_voltage(int pin, float correct_val, float allowance_percent, boolean debug)
{
  int reading = analogRead(pin);

  if (debug)
  {
    Serial.print("Reading:");
    Serial.println(reading);
  }
  
  if ((reading <= (correct_val * (1 + allowance_percent))) && (reading >= (correct_val * (1 - allowance_percent)))) 
	return true; // good value
  else 
	return false;
}

boolean FlyingJalapeno::verify_value(int input_value, int correct_val, float allowance_percent)
{
  if ((input_value <= (correct_val * (1 + allowance_percent))) && (input_value >= (correct_val * (1 - allowance_percent)))) return true; // good value
  else return false;
}

//Setup the first power supply to the chosen voltage level
//Set power_on to false to power down PSU
void FlyingJalapeno::setV1(boolean power_on, float voltage)
{

// These lines are connected to different reistors off the adj line
// Pulling pins low enables the resistors
// Turning pins to input disables the resistors
#define PSU1_VOLTAGE_CONTROL_TO_3V3 33
#define PSU1_VOLTAGE_CONTROL_TO_5V0 35

// Set this pin high to enable power supply
#define PSU1_POWER_CONTROL 48

  digitalWrite(PSU1_VOLTAGE_CONTROL_TO_3V3, LOW);
  digitalWrite(PSU1_VOLTAGE_CONTROL_TO_5V0, LOW);
  pinMode(PSU1_VOLTAGE_CONTROL_TO_3V3, INPUT);
  pinMode(PSU1_VOLTAGE_CONTROL_TO_5V0, INPUT);

  pinMode(PSU1_POWER_CONTROL, OUTPUT);
  digitalWrite(PSU1_POWER_CONTROL, LOW); // turn OFF the high side switch by default

  if (power_on == true)
  {
    if (voltage == 3.3)
    {
      pinMode(PSU1_VOLTAGE_CONTROL_TO_3V3, OUTPUT);
      digitalWrite(PSU1_VOLTAGE_CONTROL_TO_3V3, LOW);
    }
    else if (voltage == 5)
    {
      pinMode(PSU1_VOLTAGE_CONTROL_TO_5V0, OUTPUT);
      digitalWrite(PSU1_VOLTAGE_CONTROL_TO_5V0, LOW);
    }

    digitalWrite(PSU1_POWER_CONTROL, HIGH); // turn on the high side switch
  }
  else if (power_on == false)
  {
    digitalWrite(PSU1_POWER_CONTROL, LOW); // turn OFF the high side switch

    pinMode(PSU1_VOLTAGE_CONTROL_TO_3V3, OUTPUT); // default to 3.3V - even when the high side switch is turn off.
    digitalWrite(PSU1_VOLTAGE_CONTROL_TO_3V3, LOW);
  }
}

//Setup the second power supply to the chosen voltage level
//Set power_on to false to power down PSU
void FlyingJalapeno::setV2(boolean power_on, float voltage)
{
// These lines are connected to different reistors off the adj line
// Pulling pins low enables the resistors
// Turning pins to input disables the resistors
#define PSU2_VOLTAGE_CONTROL_TO_3V3 36
#define PSU2_VOLTAGE_CONTROL_TO_3V7 37
#define PSU2_VOLTAGE_CONTROL_TO_4V2 38
#define PSU2_VOLTAGE_CONTROL_TO_5V0 39

// Set this pin high to enable power supply
#define PSU2_POWER_CONTROL 49

  // lines that control the vreg setting
  // these are connected to different reistors off the adj line on the vreg
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V3, LOW);
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V7, LOW);
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_4V2, LOW);
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_5V0, LOW);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_3V3, INPUT);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_3V7, INPUT);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_4V2, INPUT);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_5V0, INPUT);

  if (power_on)
  {
    if (voltage == 3.3)
    {
      pinMode(PSU2_VOLTAGE_CONTROL_TO_3V3, OUTPUT);
      digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V3, LOW);
    }
    else if (voltage == 5)
    {
      pinMode(PSU2_VOLTAGE_CONTROL_TO_5V0, OUTPUT);
      digitalWrite(PSU2_VOLTAGE_CONTROL_TO_5V0, LOW);
    }
    else if (voltage == 4.2)
    {
      pinMode(PSU2_VOLTAGE_CONTROL_TO_4V2, OUTPUT);
      digitalWrite(PSU2_VOLTAGE_CONTROL_TO_4V2, LOW);
    }
    else if (voltage == 3.7)
    {
      pinMode(PSU2_VOLTAGE_CONTROL_TO_3V7, OUTPUT);
      digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V7, LOW);
    }
    pinMode(PSU2_POWER_CONTROL, OUTPUT);
    digitalWrite(PSU2_POWER_CONTROL, HIGH); // turn on the high side switch
  }
  else
  {
    pinMode(PSU2_POWER_CONTROL, OUTPUT);
    digitalWrite(PSU2_POWER_CONTROL, LOW); // turn OFF the high side switch

    pinMode(PSU2_VOLTAGE_CONTROL_TO_3V3, OUTPUT); // default to 3.3V - even when the high side switch is turn off.
    digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V3, LOW);
  }
}

void FlyingJalapeno::enablePCA(void)
{
  // PCA is enabled via PD4, which is not a standard arduino pin, so we will have to write this via register calls... hmmff!
  DDRD = DDRD | B00010000; // only set PD4 as output
  PORTD = PORTD | B00010000; // PD4 HIGH

  delay(100);
}

void FlyingJalapeno::disablePCA(void)
{
  // PCA is enabled via PD4, which is not a standard arduino pin, so we will have to write this via register calls... hmmff!
  
  PORTD = PORTD & ~(B00010000); // PD4 LOW - Disables the PCA
  
  delay(100);
}