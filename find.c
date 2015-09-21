/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Sun Sep 20 14:45:46 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #1.1
 * File: find.c
 * Summary:  
 *  
 *****************************************************/

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int BUFFER_SIZE = 256;

//main file to accept 
 int main() {
 	//Variables
 	char buf[BUFFER_SIZE]; //our buffer
 	char strm[BUFFER_SIZE]; //keep track of our symbols
 	//char sym[BUFFER_SIZE];
 	
	while(fgets(buf, BUFFER_SIZE, stdin) != NULL)
		strncat(strm, buf, BUFFER_SIZE); //copy contents from  buffer

	int i;
	for(i=0; i < BUFFER_SIZE; i++) {
		if(isalpha(strm[i])){
			strm[i] = toupper(strm[i]);
		}
	}

	printf("%s\n", strm);

 	return 0;
 }
