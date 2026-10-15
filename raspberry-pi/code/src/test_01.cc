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

	while (1)
	{
		if (digitalRead(buttonPin) == LOW)
		{													  // button is pressed
			digitalWrite(ledPin, HIGH);						  // Make GPIO output HIGH level
			printf("Button is pressed, led turned on >>>\n"); // Output information on terminal
			delay(5000);
		}
		else
		{														// button is released
			digitalWrite(ledPin, LOW);							// Make GPIO output LOW level
			printf("Button is released, led turned off <<<\n"); // Output information on terminal
		}
	}

	return 0;
}
