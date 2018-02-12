// Calc.h

#ifndef _CALC_h
#define _CALC_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "LedDisplay.h";
#include "Keypad.h";

using opfun = long(*)(long, long);

struct OperatorFun {
	int Name[4];
	opfun fun;
};

class Calc {
public:
	Calc(LedDisplay *display);
	Calc(int(&ledPins)[LEDS_COUNT], int(&anodes)[DIGITS_COUNT]);

	void cycleOperators();
	void applyOperator();
	void enterDigit(int digit);
	void reset();

private:
	LedDisplay *ledDisplay;
	long number;
	long lastNumber;
	int currentOperator;

	void init();
	void reduce(long &num);
};

#endif

