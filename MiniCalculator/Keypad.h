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

using Key = int;
using keyCallback = void(*)(Key);


/**
*	@brief Abstraction of the Elegoo matrix keypad
*/
class Keypad {
public:
	// Setup the Arduino pins for the keypad
	void setup(int (&hlines)[KEYPAD_LINES], int(&vlines)[KEYPAD_LINES]);

	// Returns true, when the Key is not a number, but a special key
	bool isSpecialKey(Key key);
	// Returns the currently pressed key
	Key getPressedKey();
	// Call this regularly (e.g. Arduino loop() function) and get feedback about key changes
	// over the callback functions 'keyDown' and 'keyUp'.
	void process();

	// Callback. Will be caled once a key is pressed down
	keyCallback keyDown;
	// Callback. Will be called once a key is released.
	keyCallback keyUp;

private:
	int hlinePins[KEYPAD_LINES];
	int vlinePins[KEYPAD_LINES];
	bool setup_done = false;
	Key lastKey = KEY_NONE;

	void chooseHline(int line);
};

#endif

