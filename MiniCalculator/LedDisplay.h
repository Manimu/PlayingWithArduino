// LedDisplay.h

#ifndef _LEDDISPLAY_h
#define _LEDDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
/*
   a
   ==
 f|g |b
   ==
 e|d |c
   ==
*/
enum Characters : uint8_t {
	Blank = 0x00,
	SegmA = 0x01,
	SegmB = 0x02,
	SegmC = 0x04,
	SegmD = 0x08,
	SegmE = 0x10,
	SegmF = 0x20,
	SegmG = 0x40,

	Num0 = SegmA | SegmB | SegmC | SegmD | SegmE | SegmF,
	Num1 = SegmB | SegmC,
	Num2 = SegmA | SegmB | SegmG | SegmE | SegmD,
	Num3 = SegmA | SegmB | SegmC | SegmD | SegmG,
	Num4 = SegmF | SegmG | SegmB | SegmC,
	Num5 = SegmA | SegmF | SegmG | SegmC | SegmD,
	Num6 = SegmA | SegmF | SegmE | SegmD | SegmC | SegmG,
	Num7 = SegmA | SegmB | SegmC,
	Num8 = SegmA | SegmB | SegmC | SegmD | SegmE | SegmF | SegmG,
	Num9 = SegmG | SegmF | SegmA | SegmB | SegmC | SegmD,

	A = SegmE | SegmF | SegmA | SegmB | SegmC | SegmG,
	B = SegmF | SegmE | SegmD | SegmC | SegmG,
	C = SegmA | SegmF | SegmE | SegmD,
	D = SegmG | SegmE | SegmD | SegmC | SegmB,
	E = SegmA | SegmF | SegmE | SegmD | SegmG,
	F = SegmA | SegmF | SegmE | SegmG,

	Minus = SegmG,
};

constexpr int LEDS_COUNT = 7;
constexpr int DIGITS_COUNT = 4;
constexpr int DIGITS_NONE = -1;
constexpr int MAX_LED_DISPLAYS_FOR_TIMER = 4;

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
	void setDigits(uint8_t(&d)[DIGITS_COUNT]);
	// Set the digits on the display individually
	void setDigits(uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0);
	
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
	volatile uint8_t digits[DIGITS_COUNT];
	int activeDigit;
	bool setup_done;
	
	void chooseLedBlock(int index);
	void displayDigit(uint8_t c);
};

#endif

