
#include "LedDisplay.h"
/*
	  a
	  ==
	f|g |b
	  ==
	e|d |c
	  ==
*/
static const bool Digits[][LEDS_COUNT] =
{  // a  b  c  d  e  f  g
	{ 1, 1, 1, 1, 1, 1, 0 },	//0
	{ 0, 1, 1, 0, 0, 0, 0 },	//1
	{ 1, 1, 0, 1, 1, 0, 1 },	//2
	{ 1, 1, 1, 1, 0, 0, 1 },	//3
	{ 0, 1, 1, 0, 0, 1, 1 },	//4
	{ 1, 0, 1, 1, 0, 1, 1 },	//5
	{ 1, 0, 1, 1, 1, 1, 1 },	//6
	{ 1, 1, 1, 0, 0, 0, 0 },	//7
	{ 1, 1, 1, 1, 1, 1, 1 },	//8
	{ 1, 1, 1, 1, 0, 1, 1 },	//9

	{ 1, 1, 1, 0, 1, 1, 1 },	//DIGIT_A
	{ 0, 0, 1, 1, 1, 1, 1 },	//DIGIT_B
	{ 1, 0, 0, 1, 1, 1, 0 },	//DIGIT_C
	{ 0, 1, 1, 1, 1, 0, 1 },	//DIGIT_D
	{ 1, 0, 0, 1, 1, 1, 1 },	//DIGIT_E
	{ 1, 0, 0, 0, 1, 1, 1 },	//DIGIT_F
	{ 1, 0, 1, 1, 1, 1, 0 },	//DIGIT_G
	{ 0, 1, 1, 0, 1, 1, 1 },	//DIGIT_H
	{ 0, 1, 1, 1, 1, 0, 0 },	//DIGIT_J

	{ 1, 0, 0, 0, 0, 0, 0 },	//DIGIT_SEGM_A
	{ 0, 1, 0, 0, 0, 0, 0 },	//DIGIT_SEGM_B
	{ 0, 0, 1, 0, 0, 0, 0 },	//DIGIT_SEGM_C
	{ 0, 0, 0, 1, 0, 0, 0 },	//DIGIT_SEGM_D
	{ 0, 0, 0, 0, 1, 0, 0 },	//DIGIT_SEGM_E
	{ 0, 0, 0, 0, 0, 1, 0 },	//DIGIT_SEGM_F
	{ 0, 0, 0, 0, 0, 0, 1 },	//DIGIT_SEGM_G

	{ 0, 0, 0, 0, 0, 0, 0 },	//DIGIT_BLANK
};
static const int sizeOfDigits = sizeof(Digits) / LEDS_COUNT / sizeof(bool);

LedDisplay *LedDisplays[MAX_LED_DISPLAYS_FOR_TIMER];

LedDisplay::LedDisplay() {
	for (int i = 0; i < DIGITS_COUNT; ++i)
		digits[i] = DIGIT_BLANK;

	setup_done = false;
	activeDigit = 0;
	
	for (int i = 0; i < MAX_LED_DISPLAYS_FOR_TIMER; ++i) {
		if (LedDisplays[i] == 0) {
			LedDisplays[i] = this;
			break;
		}
	}
}

void LedDisplay::setup(int (&ledPins)[LEDS_COUNT], int (&anodes)[DIGITS_COUNT]) {
	for (int i = 0; i < LEDS_COUNT; ++i) {
		this->ledPins[i] = ledPins[i];
		pinMode(this->ledPins[i], OUTPUT);
	}
	for (int i = 0; i < DIGITS_COUNT; ++i) {
		this->anodePins[i] = anodes[i];
		pinMode(this->anodePins[i], OUTPUT);
		digitalWrite(this->anodePins[i], LOW);
	}
	setup_done = true;
}

void LedDisplay::registerTimer() {
	cli();

	//set to 0.5Mhz increment (8 prescaler):
	//	OCR0A = (16*10^6) / (2000*8) - 1

	TCCR0A = 0;							// set entire TCCR0A register to 0
	TCCR0B = 0;							// same for TCCR0B
	TCNT0 = 0;							// initialize counter value to 0
	OCR0A = 255;						// must be <256
	TCCR0A |= (1 << WGM01);				// turn on CTC mode
	TCCR0B |= (1 << CS01);				// Set CS01 bit for 8 prescaler
	TIMSK0 |= (1 << OCIE0A);			// enable timer compare interrupt

	sei();
}

ISR(TIMER0_COMPA_vect) {
	for (int i = 0; i < MAX_LED_DISPLAYS_FOR_TIMER; ++i) {
		if (LedDisplays[i] == 0)
			return;

		LedDisplays[i]->refresh();
	}
}

void LedDisplay::setDigits(int(&d)[DIGITS_COUNT]) {
	setDigits(d[3], d[2], d[1], d[0]);
}

void LedDisplay::setDigits(int d3, int d2, int d1, int d0) {
	validateDigit(d0); this->digits[0] = d0;
	validateDigit(d1); this->digits[1] = d1;
	validateDigit(d2); this->digits[2] = d2;
	validateDigit(d3); this->digits[3] = d3;
}

void LedDisplay::clear() {
	setDigits(DIGIT_BLANK, DIGIT_BLANK, DIGIT_BLANK, DIGIT_BLANK);
}

void LedDisplay::setDecimal(int number) {
	bool needToSetSign = number < 0;
	int d[DIGITS_COUNT];

	for (int i = 0; i < DIGITS_COUNT; ++i) {
		int digit = abs(number) % 10;
		
		if (number == 0 && i > 0) {
			digit = DIGIT_BLANK;
			if (needToSetSign) {
				digit = DIGIT_MINUS;
				needToSetSign = false;
			}
		}
		d[i] = digit;
		number /= 10;
	}
	setDigits(d);
}

void LedDisplay::setNumber(int number, int base) {
	if (base <= 1 || base > 16)
		return;	//TODO: WHAT TO DO HERE?
	if (number < 0)
		return; //TODO: Minus handling?

	int d[DIGITS_COUNT];
	for (int i = 0; i < DIGITS_COUNT; ++i) {
		int digit = abs(number) % base;

		if (number == 0 && i > 0) {
			digit = DIGIT_BLANK;
		}
		d[i] = digit;
		number /= base;
	}
	setDigits(d);
}

void LedDisplay::setHex(int number) {
	setNumber(number, 16);
}

void LedDisplay::setOct(int number) {
	setNumber(number, 8);
}

void LedDisplay::setBin(int number) {
	setNumber(number, 2);
}

void LedDisplay::setLedTest() {
	setDigits(8, 8, 8, 8);
}

void LedDisplay::refresh() {
	if (!setup_done)
		return;

	chooseLedBlock(DIGITS_NONE);
	displayDigit(digits[activeDigit]);
	chooseLedBlock(activeDigit);

	activeDigit++;
	if (activeDigit >= DIGITS_COUNT)
		activeDigit = 0;
}

void LedDisplay::chooseLedBlock(int index) {
	for (int i = 0; i < DIGITS_COUNT; ++i)
		digitalWrite(anodePins[i], i == index);
}

void LedDisplay::displayDigit(int digit) {
	validateDigit(digit);

	for (int i = 0; i < LEDS_COUNT; ++i) 
		digitalWrite(ledPins[i], 1 != Digits[digit][i]);
}

void inline LedDisplay::validateDigit(int& digit) {
	if (digit < 0 || digit >= sizeOfDigits)
		digit = DIGIT_BLANK;
}

