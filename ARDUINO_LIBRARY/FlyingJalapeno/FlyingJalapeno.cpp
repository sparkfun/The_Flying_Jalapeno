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

  pinMode(PSU1_POWER_CONTROL, OUTPUT);
  disableRegulator1();

  pinMode(PSU2_POWER_CONTROL, OUTPUT);
  disableRegulator2();
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


//Test power circuit 1 to see if there is a short on the target
//Returns true if all is ok
//Returns false if there is a short
boolean FlyingJalapeno::testRegulator1() 
{
  return(powerTest(1));
}

boolean FlyingJalapeno::testRegulator2() 
{
  return(powerTest(2));
}

//Test target board for shorts to GND
//Returns true if all is good, returns false if there is short detected
boolean FlyingJalapeno::powerTest(byte select) // select is for either "1" or "2" for using either pretest resistors on the FJ
{
  //Power down regulators
  disableRegulator1();
  disableRegulator2();
  
  //Setup control pin
  pinMode(POWER_TEST_CONTROL, OUTPUT);
  digitalWrite(POWER_TEST_CONTROL, HIGH);

  byte read_pin;
  if (select == 1) read_pin = A14;
  else if (select == 2) read_pin = A15;

  pinMode(read_pin, INPUT);

  delay(200); //Wait for voltage to settle before taking a ADC reading

  int reading = analogRead(read_pin);

  Serial.print("Power test reading (should >500 or <471): ");
  Serial.println(reading);

  //Release the control pin
  digitalWrite(POWER_TEST_CONTROL, LOW);
  pinMode(POWER_TEST_CONTROL, INPUT);

  int jumper_val = 486;

  if ((reading < (jumper_val * 1.03)) && (reading > (jumper_val * 0.97))) 
	return false; // jumper detected!!
  else 
	return true;
}

//Test a pin to see what voltage is on the pin.
//Returns true if pin voltage is within a given window of the value we are looking for
//pin = pin to test
//expectedVoltage = voltage we expect. 0.0 to 5.0 (float)
//allowedPpercent = allowed window for overage. 0 to 100 (int) (default 10%)
//debug = print debug statements (default false)
boolean FlyingJalapeno::verifyVoltage(int pin, float expectedVoltage, int allowedPercent, boolean debug)
{
  int expectedValue = map(expectedVoltage, 0.0, 5.0, 0, 1023); //Scale float to an ADC value

  float allowanceFraction = map(allowedPercent, 0, 100, 0.0, 1.0); //Scale the allowedPercent to a float

  int reading = analogRead(pin);

  if (debug)
  {
    Serial.print("Reading:");
    Serial.println(reading);
  }
  
  if ((reading <= (expectedValue * (1 + allowanceFraction))) && (reading >= (expectedValue * (1 - allowanceFraction)))) 
	return true; // good value
  else 
	return false;
}

boolean FlyingJalapeno::verify_value(int input_value, int correct_val, float allowance_percent)
{
  if ((input_value <= (correct_val * (1 + allowance_percent))) && (input_value >= (correct_val * (1 - allowance_percent)))) return true; // good value
  else return false;
}

//Enable or disable regulator #1
void FlyingJalapeno::enableRegulator1(void)
{
    digitalWrite(PSU1_POWER_CONTROL, HIGH); // turn on the high side switch
}

void FlyingJalapeno::disableRegulator1(void)
{
    digitalWrite(PSU1_POWER_CONTROL, LOW); // turn off the high side switch
}

//Enable or disable regulator #2
void FlyingJalapeno::enableRegulator2(void)
{
    digitalWrite(PSU2_POWER_CONTROL, HIGH); // turn on the high side switch
}

void FlyingJalapeno::disableRegulator2(void)
{
    digitalWrite(PSU2_POWER_CONTROL, LOW); // turn off the high side switch
}

//Setup the first power supply to the chosen voltage level
void FlyingJalapeno::setRegulatorVoltage1(float voltage)
{
  digitalWrite(PSU1_VOLTAGE_CONTROL_TO_3V3, LOW);
  digitalWrite(PSU1_VOLTAGE_CONTROL_TO_5V0, LOW);
  pinMode(PSU1_VOLTAGE_CONTROL_TO_3V3, INPUT);
  pinMode(PSU1_VOLTAGE_CONTROL_TO_5V0, INPUT);

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
  else
  {
    pinMode(PSU1_VOLTAGE_CONTROL_TO_3V3, OUTPUT); // default to 3.3V - even when the high side switch is turn off.
    digitalWrite(PSU1_VOLTAGE_CONTROL_TO_3V3, LOW);
  }
}

//Setup the second power supply to the chosen voltage level
void FlyingJalapeno::setRegulatorVoltage2(float voltage)
{
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V3, LOW);
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V7, LOW);
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_4V2, LOW);
  digitalWrite(PSU2_VOLTAGE_CONTROL_TO_5V0, LOW);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_3V3, INPUT);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_3V7, INPUT);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_4V2, INPUT);
  pinMode(PSU2_VOLTAGE_CONTROL_TO_5V0, INPUT);

  if (voltage == 3.3)
  {
    pinMode(PSU2_VOLTAGE_CONTROL_TO_3V3, OUTPUT);
    digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V3, LOW);
  }
  else if (voltage == 3.7)
  {
    pinMode(PSU2_VOLTAGE_CONTROL_TO_3V7, OUTPUT);
    digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V7, LOW);
  }
  else if (voltage == 4.2)
  {
    pinMode(PSU2_VOLTAGE_CONTROL_TO_4V2, OUTPUT);
    digitalWrite(PSU2_VOLTAGE_CONTROL_TO_4V2, LOW);
  }
  else if (voltage == 5)
  {
    pinMode(PSU2_VOLTAGE_CONTROL_TO_5V0, OUTPUT);
    digitalWrite(PSU2_VOLTAGE_CONTROL_TO_5V0, LOW);
  }
  else
  {
    pinMode(PSU2_VOLTAGE_CONTROL_TO_3V3, OUTPUT); // default to 3.3V
    digitalWrite(PSU2_VOLTAGE_CONTROL_TO_3V3, LOW);
  }
}

//Enable or disable the I2C buffer
//If you're doing I2C you'll need to enable the buffer
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

//Depricated functions
//These are here to make old code still work. Use the newer functions
void FlyingJalapeno::PCA_enable(boolean enable)
{
  if(enable) enablePCA();
  else disablePCA();
}

//Replaced by setRegulatorVoltage1() and enableRegulator1()
void FlyingJalapeno::setV1(boolean power_on, float voltage)
{
  if(power_on) enableRegulator1();
  else disableRegulator1();
  
  setRegulatorVoltage1(voltage);
}

void FlyingJalapeno::setV2(boolean power_on, float voltage)
{
  if(power_on) enableRegulator2();
  else disableRegulator2();
  
  setRegulatorVoltage2(voltage);
}

//Power test. Replaced by powerTest
boolean FlyingJalapeno::PT(byte select) // select is for either "1" or "2" for using either pretest resistors on the FJ
{
  powerTest(select);
}

boolean FlyingJalapeno::verify_voltage(int pin, int correct_val, float allowance_percent, boolean debug)
{
  int scaledPercent = allowance_percent * 100; //verifyVoltage expects an int. Scale 0.1 to 10.
  
  float scaledVoltage = (correct_val, 0, 1023, 0.0, 5.0); //Scale voltage to a float.
  
  boolean result = verifyVoltage(pin, scaledVoltage, scaledPercent, debug);
  
  return(result);
}