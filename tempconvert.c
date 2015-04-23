#include <stdio.h>

/* Print Fahrenheit-Celsius Table */

int main (void)
{	
	// VARIABLES
	int fahr, celsius;
	int lower, upper;
	
	// INITIALIZATION
	upper = 300;
	lower = 0;

	// TASK
	printf ("-----------CONVERT TEMPERATURE-----------\n\n");
	printf ("Enter a temperature to convert to Celsius: ");
	scanf ("%d", &fahr);

	celsius = 5 * (fahr - 32) / 9;
	printf ("Fahrenheit: %d Degrees\nCelsius: %d Degrees\n", fahr, celsius);

	return 0;
} 
