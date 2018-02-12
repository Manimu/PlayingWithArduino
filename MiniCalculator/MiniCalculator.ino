/*
 Name:		MiniCalculator.ino
 Created:	2/4/2018 4:28:57 PM
 Author:	Jeremi
*/
#include "Calc.h"
#include "Keypad.h"

//                       a  b  c  d  e  f  g
int displayLedPins[] = { 4, 5, 6, 7, 8, 2, 3 };
int displayAnodePins[] = { 10, 11, 12, 9 };
int pinPadHPins[] = { A0, A1, A2, A3 };
int pinPadVPins[] = { A4, A5, 13, KEYPAD_PIN_UNASSIGNED };

#define KEY_OPERATOR_APPLY		KEY_HASH
#define KEY_OPERATOR_CYCLE		KEY_STAR
#define RESET_TIME				2000     
long lastDownTime = 0;


Calc *calc;
Keypad keypad;

void setup() {
	keypad.setup(pinPadHPins, pinPadVPins);
	keypad.keyDown = keypad_keyDown;
	keypad.keyUp = keypad_keyUp;

	calc = new Calc(displayLedPins, displayAnodePins);
}

void loop() {
	keypad.process();
}


void keypad_keyDown(Key key) {
	lastDownTime = millis();

	if (key == KEY_OPERATOR_APPLY) {
		calc->applyOperator();
		return;
	}

	if (key == KEY_OPERATOR_CYCLE) {
		calc->cycleOperators();
		return;
	}

	if (0 <= key && key <= 9) {
		calc->enterDigit(key);
		return;
	}
}

void keypad_keyUp(Key key) {
	if (key != KEY_OPERATOR_CYCLE)
		return;
	if (millis() - lastDownTime < RESET_TIME)
		return;

	calc->reset();
}