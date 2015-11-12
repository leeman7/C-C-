/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Sat Sep 26 23:21:11 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #2
 * File: shhh.c
 * Summary:  Shell
 *  
 *****************************************************/

/* LIBRARIES */
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>

/* FUNCTIONS */
void reset_var(int argc, int inredir, int outredir, int pipes){

	argc = 0;
	inredir = 0;
	outredir = 0;
	pipes = 0;
}

//MAIN
main(){

	/* VARIABLES */
	char *argv[20], buf[80], n, *p;
	int m, status, inword, continu, output, input;;
	int pipes=0; //process ids FOR TESTING
	int left_fd[2], right_fd[2];
	int k=0, argc=0, inredir=0, outredir=0;
	pid_t pid;

	while(1){
		//initialization of shell
		inword = 0;
		p = buf;
		m = 0;
		continu=0;
		printf( "\nshell> ");

		//get command while newline does not exist
		while ((n=getchar()) != '\n' || continu){
			//if n equal to whitespace check word
			if (n ==  ' '){ 
				//if no content reset inword increment p
				if (inword){
					inword = 0;
					*p++ = 0;
				} 
			}
			//if newline is read reset continu aka end of command
			else if (n == '\n')
				continu = 0;
			//check if \\ and if inword does not exist then increment command
			else if (n == '\\' && !inword)
				continu = 1;
			else { //default check 
				if (!inword){
					inword = 1;
					argv[m++] = p;
					*p++ = n;
					argc++;
				} else { 
					*p++ = n;
				}
			}
		}

		*p++ = 0;
		argv[m] = 0;

		if(argc > 0){

		//if command quit entered then exit
		if(strcmp(argv[0],"quit") == 0)
			exit(0);

		//change directory
		if(strcmp(argv[0], "cd") == 0){
			chdir(argv[1]);
			continue;
		} else {
			//check number of pipes and replace with '0'
			//Preprocessing 
			int i, amp = 0;
			int *location = malloc(sizeof(int[32]));
			location[k++] = 0;

			for(i=0; i<m; i++){
				if(*argv[i] == '|'){
					location[k] = i+1;
					*argv[i] = 0;
					pipes++;
				}
				if(*argv[i] == '<'){
					*argv[i] = 0;
					inredir = 1;
					input = i+1;
				}
				if(*argv[i] == '>'){
					*argv[i] = 0;
					outredir = 1;
					output = i+1;
				}
				if(*argv[i] == '&'){
					*argv[i] = 0;
					amp = 1;
				}
			}

			/* Forking Processes */
			int j;
			int ifd,ofd;
			for(j=0; j <= pipes; j++){
				if(pipes != 0){
					pipe(right_fd);
				}

				pid = fork();

				if(pid == 0){
					//Child

					//check input redirection
					if(j == 0 && inredir > 0){
						ifd=open(argv[input], STDIN_FILENO, O_RDONLY, O_TRUNC, 0666);
						if(dup2(ifd, 0) == 1)
							close(ifd);
					}
					//check command for pipes
					if(pipes>0){
						if(j == 0){ //First
							close(1);
							dup2(right_fd[1], 1);
						} else if(j == (pipes+1)){ //Last
							close(0);
							dup2(left_fd[1],1);
							close(left_fd[0]);
						} else {  //Middle
							close(0);
							dup2(left_fd[0],0);
							close(left_fd[1]);

							close(1);
							dup2(right_fd[1],1);
							close(right_fd[0]);
						}
					}
					//check output redirection
					if(j == pipes && outredir > 0){
						ofd=open(argv[output], O_CREAT | O_WRONLY | O_TRUNC, 0666);
						if(dup2(ofd,1) < 0)
							close(ofd);
					}
				
					execvp(argv[location[j]], &argv[location[j]]);
					printf("ERORR: execvp(c) Failed\n");
				} else if(pid > 0){ //Parent
					if(pipes>0){
						close(left_fd[0]);
						close(left_fd[1]);
					}
					left_fd[0] = right_fd[0];
					left_fd[1] = right_fd[1];

					wait(&status); //wait control for while(1)
				} else { //Error
					perror("ERROR: Fork failed");
				}
			}
		}
	}

	//reset counters
	reset_var(argc,inredir,outredir,pipes);

	}
}