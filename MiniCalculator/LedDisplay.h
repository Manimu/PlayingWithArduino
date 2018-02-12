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

class LedDisplay {
public:
	LedDisplay();
	void setup(int (&ledPins)[LEDS_COUNT], int (&anodes)[DIGITS_COUNT]);

	/* use refreh in your loop(), when not using the registerTimer() */
	void refresh();
	/* registerTimer() in your setup(), when you do not want to refresh manually in loop */
	void registerTimer();

	void setDigits(int(&d)[DIGITS_COUNT]);
	void setDigits(int d3, int d2, int d1, int d0);
	
	void clear();
	void setDecimal(int number);
	void setNumber(int number, int base);
	void setHex(int number);
	void setOct(int number);
	void setBin(int number);
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

