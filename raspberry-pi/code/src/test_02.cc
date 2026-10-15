// LED + Button
#include <wiringPi.h>
#include <stdio.h>

#define ledPin 0	// define the ledPin
#define buttonPin 1 // define the buttonPin

int main(void)
{
	printf("Program is starting ... \n");

	wiringPiSetup();

	pinMode(ledPin, OUTPUT);   // Set ledPin to output
	pinMode(buttonPin, INPUT); // Set buttonPin to input

	pullUpDnControl(buttonPin, PUD_UP); // pull up to HIGH level

	// First of all, read the status
	int btnStatus = digitalRead(buttonPin);
	// last recorded value of the button
	int btnStatusOld = btnStatus;
	// check if the button has been released before
	bool reset = true;
	// status of the LED
	bool isLEDon = false;
	// last time we changed value
	long lastChangeTime;

	while (1)
	{
		// Check if the status changed
		btnStatus = digitalRead(buttonPin);

		// if button change, record the time
		if(btnStatus!=btnStatusOld)
		{
			lastChangeTime = millis();
		}
		if ((btnStatus == LOW) && reset)
		{
			reset = false;
			if (!isLEDon)
			{
				isLEDon = true;
				digitalWrite(ledPin, HIGH);						  // Make GPIO output HIGH level
				printf("Button is pressed, led turned on >>>\n"); // Output information on terminal
			}
			else
			{
				isLEDon = false;
				digitalWrite(ledPin, LOW);						   // Make GPIO output HIGH level
				printf("Button is pressed, led turned off >>>\n"); // Output information on terminal
			}
		}
		else if (btnStatus == HIGH)
		{
			reset = true;
		}
	}

	return 0;
}
