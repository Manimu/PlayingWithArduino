// Keypad.h

#ifndef _KEYPAD_h
#define _KEYPAD_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define KEYPAD_LINES 4
#define KEYPAD_PIN_UNASSIGNED -1

#define KEY_NONE	-1
#define KEY_SPECIAL	10
#define KEY_A		(KEY_SPECIAL + 0)
#define KEY_B		(KEY_SPECIAL + 1)
#define KEY_C		(KEY_SPECIAL + 2)
#define KEY_D		(KEY_SPECIAL + 3)
#define KEY_STAR	(KEY_SPECIAL + 4)
#define KEY_HASH	(KEY_SPECIAL + 5)

typedef int Key;
//typedef void(*keyCallback)(Key);
using keyCallback = void(*)(Key);

class Keypad {
public:
	void setup(int (&hlines)[KEYPAD_LINES], int(&vlines)[KEYPAD_LINES]);
	bool isSpecialKey(Key key);
	Key getPressedKey();

	void process();
	keyCallback keyDown;
	keyCallback keyUp;

private:
	int hlinePins[KEYPAD_LINES];
	int vlinePins[KEYPAD_LINES];
	bool setup_done = false;
	Key lastKey = KEY_NONE;

	void chooseHline(int line);
};

#endif

