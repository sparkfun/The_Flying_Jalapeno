/*
  Morse.h - Library for flashing Morse code.
  Created by David A. Mellis, November 2, 2007.
  Released into the public domain.
*/
#ifndef FlyingJalapeno_h
#define FlyingJalapeno_h

#include "Arduino.h"

#define LED_PT_PASS 43
#define LED_PT_FAIL 42
#define LED_PASS 41
#define LED_FAIL 40

class FlyingJalapeno
{
  public:
    FlyingJalapeno(int pin);
	boolean verify_voltage(int pin, float correct_val, float allowance_percent, boolean debug);
	boolean verify_value(int input_value, int correct_val, float allowance_percent);
	boolean PT(byte select);
	void setV1(boolean on, float voltage);
	void setV2(boolean on, float voltage);
	void PCA_enable(boolean enable);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif