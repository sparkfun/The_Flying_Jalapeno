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
	byte control_pin = 44;
	
	byte read_pin;
	if(select == 1) read_pin = A14;
	else if(select == 2) read_pin = A15;

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

void FlyingJalapeno::setV1(boolean on, float voltage)
{
  // lines that control the vreg setting
  // these are connected to different reistors off the adj line on the vreg
  digitalWrite(33, LOW);
  digitalWrite(35, LOW);
  pinMode(33,INPUT);
  pinMode(35,INPUT);

  // Ensure power on that line is off
  // power control line is controlling the high side switch, post vreg for v1
  pinMode(48,OUTPUT);
  digitalWrite(48, LOW);
  
  if(on)
  {
    if(voltage == 3.3)
    {
      pinMode(33,OUTPUT);
      digitalWrite(33,LOW);
    }
    else if(voltage == 5)
    {
      pinMode(35,OUTPUT);
      digitalWrite(35,LOW);
    }

    digitalWrite(48,HIGH); // turn on the high side switch
  }
}

void FlyingJalapeno::setV2(boolean on, float voltage)
{
  // lines that control the vreg setting
  // these are connected to different reistors off the adj line on the vreg
  digitalWrite(36, LOW);
  digitalWrite(37, LOW);
  digitalWrite(38, LOW);
  digitalWrite(39, LOW);
  pinMode(36,INPUT);
  pinMode(37,INPUT);
  pinMode(38,INPUT);
  pinMode(39,INPUT);

  // Ensure power on that line is off
  // power control line is controlling the high side switch, post vreg for v2
  pinMode(49,OUTPUT);
  digitalWrite(49, LOW);
  
  if(on)
  {
    if(voltage == 3.3)
    {
      pinMode(36,OUTPUT);
      digitalWrite(36,LOW);
    }
    else if(voltage == 5)
    {
      pinMode(39,OUTPUT);
      digitalWrite(39,LOW);
    }
    else if(voltage == 4.2)
    {
      pinMode(38,OUTPUT);
      digitalWrite(38,LOW);
    }
    else if(voltage == 3.7)
    {
      pinMode(37,OUTPUT);
      digitalWrite(37,LOW);
    }

    digitalWrite(49,HIGH); // turn on the high side switch
  }
}