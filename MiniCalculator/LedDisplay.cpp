
#include "LedDisplay.h"

static const uint8_t CharactersLookup[]{
	Num0, 
	Num1, Num2, Num3, 
	Num4, Num5, Num6, 
	Num7, Num8, Num9,
	A, B, C, D, E, F,
};
static const int numOfCharactersInLookup = sizeof(CharactersLookup) / sizeof(uint8_t);

LedDisplay *LedDisplays[MAX_LED_DISPLAYS_FOR_TIMER];

LedDisplay::LedDisplay() {
	for (int i = 0; i < DIGITS_COUNT; ++i)
		digits[i] = Characters::Blank;

	setup_done = false;
	activeDigit = 0;
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

	//register this class for ISR update
	for (int i = 0; i < MAX_LED_DISPLAYS_FOR_TIMER; ++i) {
		if (LedDisplays[i] == 0 && LedDisplays[i] != this) {
			LedDisplays[i] = this;
			break;
		}
	}
}

ISR(TIMER0_COMPA_vect) {
	for (int i = 0; i < MAX_LED_DISPLAYS_FOR_TIMER; ++i) {
		if (LedDisplays[i] == 0)
			return;

		LedDisplays[i]->refresh();
	}
}

void LedDisplay::setDigits(uint8_t(&d)[DIGITS_COUNT]) {
	setDigits(d[3], d[2], d[1], d[0]);
}

void LedDisplay::setDigits(uint8_t d3, uint8_t d2, uint8_t d1, uint8_t d0) {
	digits[0] = d0;
	digits[1] = d1;
	digits[2] = d2;
	digits[3] = d3;
}

void LedDisplay::clear() {
	setDigits(Blank, Blank, Blank, Blank);
}

void LedDisplay::setDecimal(int number) {
	bool needToSetSign = number < 0;
	uint8_t d[DIGITS_COUNT];

	for (int i = 0; i < DIGITS_COUNT; ++i) {
		if (number == 0 && i > 0) {
			d[i] = Characters::Blank;
			if (needToSetSign) {
				d[i] = Characters::Minus;
				needToSetSign = false;
			}
		}
		else {
			d[i] = CharactersLookup[abs(number) % 10];
		}
		number /= 10;
	}
	setDigits(d);
}

void LedDisplay::setNumber(int number, int base) {
	if (base <= 1 || base > numOfCharactersInLookup)
		return;	//TODO: WHAT TO DO HERE?
	if (number < 0)
		return; //TODO: Minus handling?

	uint8_t d[DIGITS_COUNT];
	for (int i = 0; i < DIGITS_COUNT; ++i) {
		if (number == 0 && i > 0) 
			d[i] = Characters::Blank;
		else 
			d[i] = CharactersLookup[(number) % base];

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
	setDigits(Num8, Num8, Num8, Num8);
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

void LedDisplay::displayDigit(uint8_t c) {
	//invert c, because we need to output LOW to switch ON the Led
	for (int i = 0; i < LEDS_COUNT; ++i) {
		digitalWrite(ledPins[i], ~c & (1 << i));
	}
}
