#include <stdio.h>

int main()
{
	double Ts, speed;
	int core;
	
	printf ("----------------AMDAHL'S LAW------------------\n");

	printf ("Enter Sequential Time -Please use a percentage aka .5 for 50: ");
	scanf ("%lf", &Ts);

	printf ("\nEnter the number of cores on your system: ");
        scanf ("%d", &core);

	// Equation for Amdahls law
	speed = 1/(Ts + ((1 - Ts)/core));

	printf ("\nCalculating Amdahl's Coefficient: %lf", speed);

	return 0;
}
