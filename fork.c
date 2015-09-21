/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Sun Sep 20 14:45:46 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #1.4
 * File: fork.c
 * Summary:  
 *  
 *****************************************************/

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <sysexits.h>

// main
int main(int argc, char * argv[]) {

	// Variables
	int parent, child;
	pid_t pid;

	// Check for invalid args
	if(argc < 2 || argc > 4) {
		perror("ERROR: Too few or too many args");
		_exit(EX_USAGE);
	} else {
		// Fork Process
		pid = fork();
		parent = getppid();
		child = getpid();
	}

	switch(pid){
		case -1:
			// Fork Failed
        	perror("ERROR: fork() Failed\n");
        	_exit(EX_OSERR);
        	break;
        case 0:
        	// Child Process
        	execlp(argv[1], argv[1], argv[2], argv[3], (char *)0);
        	perror("ERROR: execlp() Failed\n");
        	break;
        default:
        	// Parent Process
        	wait(NULL); //Wait till child returns -1
        	printf("Parent Process: %d\nChild Process: %d\n", parent, child);
        	_exit(0); //kill parent
        	break;
	}

    return 0;
}