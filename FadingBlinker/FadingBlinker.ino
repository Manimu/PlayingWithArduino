/*
 Name:		FadingBlinker.ino
 Created:	2/12/2018 10:12:35 PM
 Author:	Jeremi
*/

int ledPin = 3;
int analogPin = A0;

int oldDelayTime;
int delayTime = 500;

// the setup function runs once when you press reset or power the board
void setup()
{
	oldDelayTime = delayTime;

	pinMode(ledPin, OUTPUT);
	//Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	for (int i = 0; i <= 255; ++i)
		SetLedOutput(i);

	for (int i = 255; i >= 0; --i)
		SetLedOutput(i);
}


void SetLedOutput(int i) {
	SetDelayTime();

	analogWrite(ledPin, i);
	delayMicroseconds(delayTime);
}

void SetDelayTime() {
	int value = analogRead(analogPin);
	delayTime = value + 500;

	//if (abs(oldDelayTime - delayTime) > 3) {
	//Serial.println(delayTime);
	oldDelayTime = delayTime;
	//}
}
