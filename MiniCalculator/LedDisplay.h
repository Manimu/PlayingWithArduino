// LedDisplay.h

#ifndef _LEDDISPLAY_h
#define _LEDDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define LEDS_COUNT 7
#define DIGITS_COUNT 4
#define DIGITS_NONE -1

#define DIGIT_A 10
#define DIGIT_B (DIGIT_A + 1)
#define DIGIT_C (DIGIT_B + 1)
#define DIGIT_D (DIGIT_C + 1)
#define DIGIT_E (DIGIT_D + 1)
#define DIGIT_F (DIGIT_E + 1)
#define DIGIT_G (DIGIT_F + 1)
#define DIGIT_H (DIGIT_G + 1)
#define DIGIT_I 1
#define DIGIT_J (DIGIT_H + 1)
#define DIGIT_S 5
#define DIGIT_T 7

#define DIGIT_SEGM_A (DIGIT_J + 1)
#define DIGIT_SEGM_B (DIGIT_SEGM_A + 1)
#define DIGIT_SEGM_C (DIGIT_SEGM_A + 2)
#define DIGIT_SEGM_D (DIGIT_SEGM_A + 3)
#define DIGIT_SEGM_E (DIGIT_SEGM_A + 4)
#define DIGIT_SEGM_F (DIGIT_SEGM_A + 5)
#define DIGIT_SEGM_G (DIGIT_SEGM_A + 6)

#define DIGIT_BLANK (DIGIT_SEGM_G + 1)
#define DIGIT_MINUS DIGIT_SEGM_G

#define MAX_LED_DISPLAYS_FOR_TIMER 4

/**
*	@brief Abstraction of a display build out of 7-Segment LED blocks. The blocks must have a
*		common anode. Multiplexing will be managed by this class. There are two modes of operation:
*
*	1) Either call the refresh()-function regularly, e.g. in the Arduino's loop(). This can lead to
*		different dim levels of the display, depending on the time, the loop() needs to process all
*		the other statements, before calling refresh() again.
*	
*		OR
*	
*	2) Call once the registerTimer()-function. The display's refresh will then be taken over by Arduinos
*		internal timer. Do not call refresh() in this case. Be aware, that in this case, the AD pins will
*		stop working!
*/
class LedDisplay {
public:
	// Constructor
	LedDisplay();

	/**
	*	@brief Setup LedDislay with the used pins.
	*
	*	@param ledPins is an array of the led cathodes in order a, b, c, d, e, f, g.
	*	@param anodes is an array of the anodes of each Led 7 segment block.
	*/
	void setup(int (&ledPins)[LEDS_COUNT], int (&anodes)[DIGITS_COUNT]);
	// Refresh the display. Use refreh in your loop(), when not using registerTimer()
	void refresh();
	// registerTimer() in your setup(), when you do not want to refresh manually in loop
	void registerTimer();

	// Set the digits on the display via an array.
	void setDigits(int(&d)[DIGITS_COUNT]);
	// Set the digits on the display individually
	void setDigits(int d3, int d2, int d1, int d0);
	
	// Clears the display (all LEDs off), inverse of setLedTest().
	void clear();
	// Set the decimal number to be shown on the display. This function supports negative numbers.
	void setDecimal(int number);
	// Set a number with arbitrary base on the display. 
	void setNumber(int number, int base);
	// Set a hexadecimal number to be shown on the display.
	void setHex(int number);
	// Set a octal number to be shown on the display.
	void setOct(int number);
	// Set a binary number to be shown on the display;.
	void setBin(int number);
	// Enable all LEDs, inverse of clear().
	void setLedTest();

private:
	int ledPins[LEDS_COUNT];
	int anodePins[DIGITS_COUNT];
	volatile int digits[DIGITS_COUNT];
	int activeDigit;
	bool setup_done;
	
	void chooseLedBlock(int index);
	void displayDigit(int digit);
	void inline validateDigit(int& digit);
};

#endif

