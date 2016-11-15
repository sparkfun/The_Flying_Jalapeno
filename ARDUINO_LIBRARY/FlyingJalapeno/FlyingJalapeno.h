
#ifndef FlyingJalapeno_h
#define FlyingJalapeno_h

#include "Arduino.h"

//These are the four LEDs on the test jig
//Setting these pins high turn on a given LED
#define LED_PT_PASS 43 //PT = Pre-test
#define LED_PT_FAIL 42 
#define LED_PRETEST_PASS 43
#define LED_PRETEST_FAIL 42 
#define LED_PASS 41
#define LED_FAIL 40


// These lines are connected to different resistors off the adj line
// Pulling pins low enables the resistors
// Turning pins to input disables the resistors
#define PSU1_VOLTAGE_CONTROL_TO_3V3 33
#define PSU1_VOLTAGE_CONTROL_TO_5V0 35

#define PSU2_VOLTAGE_CONTROL_TO_3V3 36
#define PSU2_VOLTAGE_CONTROL_TO_3V7 37
#define PSU2_VOLTAGE_CONTROL_TO_4V2 38
#define PSU2_VOLTAGE_CONTROL_TO_5V0 39

// Set this pin high to enable power supply
#define PSU1_POWER_CONTROL 48
#define PSU2_POWER_CONTROL 49

class FlyingJalapeno
{
  public:
    FlyingJalapeno(int statLED);

	//Returns true if pin voltage is within a given window of the value we are looking for
	boolean verify_voltage(int pin, float correct_val, float allowance_percent, boolean debug); 
	
	boolean verify_value(int input_value, int correct_val, float allowance_percent);

	boolean powerTest(byte select);

	void setRegulatorVoltage1(float voltage); //Set board voltage 1 (5 or 3.3V)
	void setRegulatorVoltage2(float voltage); //Set board voltage 2 (3.3, 3.7, 4.2, or 5V)

	//Enable or disable the power regulators
	void enableRegulator1();
	void disableRegulator1();
	void enableRegulator2();
	void disableRegulator2();

	void enablePCA(void);
	void disablePCA(void);

    void dot();
    void dash();
	
	//Depricated, don't use. These are here to allow old code to compile
	boolean PT(byte select);
	void setV1(boolean power_on, float voltage);
	void setV2(boolean power_on, float voltage);
	void PCA_enable(boolean enable);
	
  private:
    int _statLED;
};

#endif