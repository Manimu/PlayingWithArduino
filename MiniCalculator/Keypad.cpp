
#include "Keypad.h"

const int Keys[KEYPAD_LINES][KEYPAD_LINES] = 
{
	{ 1, 2, 3, KEY_A },
	{ 4, 5, 6, KEY_B },
	{ 7, 8, 9, KEY_C },
	{ KEY_STAR, 0, KEY_HASH, KEY_D },
};

void Keypad::setup(int(&hlines)[KEYPAD_LINES], int(&vlines)[KEYPAD_LINES]) {
	for (int i = 0; i < KEYPAD_LINES; ++i) {
		vlinePins[i] = vlines[i];
		if (vlinePins[i] != KEYPAD_PIN_UNASSIGNED) {
			pinMode(vlinePins[i], INPUT);
			digitalWrite(vlinePins[i], INPUT_PULLUP);
		}

		hlinePins[i] = hlines[i];
		if (hlinePins[i] != KEYPAD_PIN_UNASSIGNED) {
			pinMode(hlinePins[i], OUTPUT);
			digitalWrite(hlinePins[i], HIGH);
		}
	}
	setup_done = true;
}

bool Keypad::isSpecialKey(Key key) {
	return (key < 0 || key >= KEY_SPECIAL);
}

void Keypad::process() {
	Key currentKey = getPressedKey();
	
	if (lastKey == KEY_NONE && currentKey != KEY_NONE)
		if (keyDown != 0)
			keyDown(currentKey);
	
	if (lastKey != KEY_NONE && currentKey == KEY_NONE)
		if (keyUp != 0)
			keyUp(lastKey);
	
	lastKey = currentKey;
}

Key Keypad::getPressedKey() {
	if (!setup_done)
		return KEY_NONE;

	for (int h = 0; h < KEYPAD_LINES; ++h) {
		if (hlinePins[h] == KEYPAD_PIN_UNASSIGNED)
			continue;
		chooseHline(h);

		for (int v = 0; v < KEYPAD_LINES; ++v) {
			if (vlinePins[v] == KEYPAD_PIN_UNASSIGNED)
				continue;

			if (digitalRead(vlinePins[v]) == LOW)
				return Keys[h][v];
		}
	}
	return KEY_NONE;
}

void Keypad::chooseHline(int line) {
	for (int h = 0; h < KEYPAD_LINES; ++h) {
		if (hlinePins[h] == KEYPAD_PIN_UNASSIGNED)
			continue;

		digitalWrite(hlinePins[h], h != line);
	}
}