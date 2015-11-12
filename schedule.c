/*
 *Project 4   

You are to write a program that takes a reference string and the number
of frames as inputs. Suppose the maximum length of a reference string is 30 and
the maximum number of frames is 8. Find out the different number of page faults 
using Optimal, LRU, Most Frequently used, and Least frequently used 
algorithms. If there is a tie, use FIFO algorithm to break the tie.


Submission:

In order not to lose any files, you'd better zip all your files into a .zip file. 
Submit your project to TRACS. You should write a readme textfile telling the grader 
how to run your programs. Without this file, it is very likely that your project
will not be run properly. 
 *
 */

/* LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* FUNCTIONS */
int LRU(){}
int OPT(){}
int LFU(){}
int MFU(){}

/* MAIN */
int main(int argc, char * argv[]){

	char REF[31];
	int FRAMES[8];
	int size = strlen(argv[1]);

	if(argc < 1 || argc > 2)
		printf("ERROR: Invalid arguments\n");
	else {
		FRAMES = atoi(argv[2]);
		if(FRAMES < 1 || FRAMES > 8)
			printf("ERROR: Invalid Frame Size\n");
		if(strlen(argv[1]) > 1 || strlen(argv[1]) < 30){
			strcpy(REF, argv[1]);
		} else
			printf("ERROR: Invalid Reference String\n", );
	}


	printf("LRU: %s\n", );
	printf("OPT: %s\n", );
	printf("LFU: %s\n", );
	printf("MFU: %s\n", );

	return 0;
}