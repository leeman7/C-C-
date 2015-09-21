/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Sun Sep 20 14:45:46 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #1.3
 * File: pipe.c
 * Summary:  
 *  
 *****************************************************/

// LIBRARIES
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include <errno.h>
#include <sysexits.h>
#include <sys/types.h>

// Main
int main(int argc, char * argv[]) {

	// Variables
	int fd[2];
	pid_t pid;

	// Create Pipe Descriptor
	pipe(fd);
	if (pipe(fd) == -1) {
		perror("ERROR: Failed to allocate Pipes");
		exit(EXIT_FAILURE);
	}

	// Check for Invalid args
	if(argc < 2 || argc > 4) {
		perror("ERROR: Too few or too many args");
		_exit(EX_USAGE);
	} else {
		// Fork Process
		pid = fork();
	}

	// Check Parent and Child process
	switch(pid) {
		case -1: // Fork Failed
        	perror("ERROR: fork() Failed\n");
        	_exit(EX_OSERR);
			break;
		case 0: // Child Process
    		// Close Pipes
    		close(fd[0]);
    		dup2(fd[1], 1);

    		// execlp() command 2
    		execlp(argv[2], argv[2], (char *)0);

    		// Return error is execlp fails
        	perror("ERROR: exec() Failed\n");
        	_exit(126);
			break;
		default: // Parent Process
			//Wait till child returns -1
			wait(NULL);

        	// Close Pipes
        	close(fd[1]);
        	dup2(fd[0], 0);

        	// execlp() command 1
        	execlp(argv[1], argv[1], (char *)0);

        	// Exit Parent
        	exit(0);
			break;
	}

    return 0;
}