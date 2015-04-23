#include <stdio.h>

int main()
{
	int upper, lower, sum;
	
	printf ("--------------CALCULATE GAUSS SUM -------------\n");
	printf ("Enter a lower bound: ");
	scanf ("%d", &lower);

	printf("\nEnter an upper bound: ");
	scanf ("%d", &upper);

	sum = ((upper-lower)*((upper-lower)+1))/2;

	printf ("\nThe Gauss Sum::%d\n", sum);

	return 0;
}
