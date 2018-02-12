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

/**
*	@brief A mini calculator, that works only with numerical keys and two extra buttons.
*/
class Calc {
public:
	Calc(LedDisplay *display);
	Calc(int(&ledPins)[LEDS_COUNT], int(&anodes)[DIGITS_COUNT]);

	// Cycle through the operators
	void cycleOperators();
	// Apply the current operator and show the result
	void applyOperator();
	// Enter a digit (append to current number)
	void enterDigit(int digit);
	// Reset calculator
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

