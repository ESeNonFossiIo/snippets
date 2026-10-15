// Single LED
#include <wiringPi.h>
#include <stdio.h>

#define ledPin 0 // define the led pin number

int main(void)
{
	printf("\n Program is starting ...\n");

	wiringPiSetup(); // Initialize wiringPi.

	pinMode(ledPin, OUTPUT);
	printf("\n Using pin%d", ledPin);

	printf("\n test_00\n");
	for (int i = 0; i < 10; ++i)
	{
		printf("\n run %d", i);
		
		digitalWrite(ledPin, HIGH);
		printf("\n led turned on >>>");
		delay(1000);

		digitalWrite(ledPin, LOW);
		printf("\n led turned off <<<\n");
		delay(1000);
	}
	printf("\n");

	return 0;
}