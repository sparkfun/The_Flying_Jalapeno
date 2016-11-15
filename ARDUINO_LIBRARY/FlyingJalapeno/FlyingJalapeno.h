
#ifndef FlyingJalapeno_h
#define FlyingJalapeno_h

#include "Arduino.h"

//These are the four LEDs on the test jig
//Setting these pins high turn on a given LED
#define LED_PT_PASS 43 //PT = Pre-test
#define LED_PT_FAIL 42 
#define LED_PASS 41
#define LED_FAIL 40

class FlyingJalapeno
{
  public:
    FlyingJalapeno(int pin);
	boolean verify_voltage(int pin, float correct_val, float allowance_percent, boolean debug);
	boolean verify_value(int input_value, int correct_val, float allowance_percent);
	boolean powerTest(byte select);
	void setVoltage1(boolean on, float voltage);
	void setV2(boolean on, float voltage);
	void PCA_enable(boolean enable);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif