/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef FlyingJalapeno_h
#define FlyingJalapeno_h

#include "Arduino.h"


class FlyingJalapeno
{
  public:
    FlyingJalapeno(int pin);
	boolean verify_voltage(int pin, float correct_val, float allowance_percent, boolean debug);
	boolean verify_value(int input_value, int correct_val, float allowance_percent);
	boolean PT(byte select);
	void setV1(boolean on, float voltage);
	void setV2(boolean on, float voltage);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif