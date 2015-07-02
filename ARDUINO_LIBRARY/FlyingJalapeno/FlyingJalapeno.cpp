/*
  FlyingJalapeno.cpp - Library for SparkFun Qc's general testbed board, The Flying Jalapeno
  Created by Pete Lewis, June 29th, 2015
  Released into the public domain.
*/

#include "Arduino.h"
#include "FlyingJalapeno.h"


FlyingJalapeno::FlyingJalapeno(int pin)
{
	pinMode(pin, OUTPUT);
	_pin = pin;	
	
}

void FlyingJalapeno::dot()
{
	digitalWrite(_pin, HIGH);
	delay(250);
	digitalWrite(_pin, LOW);
	delay(250);  
}

void FlyingJalapeno::dash()
{
	digitalWrite(_pin, HIGH);
	delay(1000);
	digitalWrite(_pin, LOW);
	delay(250);
}

// PRE-TEST for shorts to GND on power rails, returns true if all is good, returns false if there is short detected
boolean FlyingJalapeno::PT(byte select) // select is for either "1" or "2" for using either pretest resistors on the FJ
{
	byte control_pin = 2;
	
	byte read_pin;
	if(select == 1) read_pin = 3;
	else if(select == 2) read_pin = 4;

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
	
	if((reading < (jumper_val*1.03)) && (reading > (jumper_val*0.97))) return false; // jumper detected!!
	else return true;
}

boolean FlyingJalapeno::verify_voltage(int pin, float correct_val, float allowance_percent, boolean debug)
{
	int reading = analogRead(pin);
	if(debug) {Serial.print("Reading:"); Serial.println(reading);}
	if((reading <= (correct_val * (1+allowance_percent))) && (reading >= (correct_val * (1-allowance_percent)))) return true; // good value
	else return false;  
}

boolean FlyingJalapeno::verify_value(int input_value, int correct_val, float allowance_percent)
{
	if((input_value <= (correct_val * (1+allowance_percent))) && (input_value >= (correct_val * (1-allowance_percent)))) return true; // good value
	else return false;  
}