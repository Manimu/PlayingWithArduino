// 
// 
// 

#include "Calc.h"

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember))

static OperatorFun operators[] =
{
	OperatorFun{
		{ A, D ,D ,Blank },
		[](long a, long b) { return a + b; }
	},
	OperatorFun{
		{ Num5, Num7, A, A },
		[](long a, long b) {return a * b; }
	},
	OperatorFun{
		{ D, E, C, Blank },
		[](long a, long b) {return a - b; }
	},
	OperatorFun{
		{ Blank, D, Num1, Blank },
		[](long a, long b) {return a / b; }
	}
};
static int sizeOfOpeartors = sizeof(operators) / sizeof(OperatorFun);


Calc::Calc(int(&ledPins)[LEDS_COUNT], int(&anodes)[DIGITS_COUNT]) {
	ledDisplay = new LedDisplay;
	ledDisplay->setup(ledPins, anodes);
	ledDisplay->registerTimer();

	init();
}

Calc::Calc(LedDisplay *display) {
	ledDisplay = display;
	init();
}

void Calc::init() {
	number = 0;
	lastNumber = 0;
	currentOperator = 0;

	ledDisplay->setDecimal(0);
}


void Calc::cycleOperators() {
	currentOperator++;
	if (currentOperator >= sizeOfOpeartors)
		currentOperator = 0;

	int d[4];
	for (int i = 0; i < 4; ++i)
		d[i] = operators[currentOperator].Name[i];

	ledDisplay->setDigits(d[0], d[1], d[2], d[3]);
}

void Calc::applyOperator() {
	lastNumber = operators[currentOperator].fun(lastNumber, number);
	reduce(lastNumber);
	number = 0;

	ledDisplay->setDecimal(lastNumber);
}

void Calc::enterDigit(int digit) {
	number *= 10;
	reduce(number);
	number += digit;

	ledDisplay->setDecimal(number);
}

void Calc::reset() {
	number = 0;
	lastNumber = 0;
	currentOperator = 0;

	ledDisplay->setDecimal(number);
}

void Calc::reduce(long &num) {
	while (number >= 10000)
		number -= 10000;
}
