/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Mon Oct 12 15:32:11 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #3
 * File: thread.c
 * Summary:  thread
 *  
 *****************************************************/

/* LIBRARIES */
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

/* DEFINES */
#define MAX_THREADS 4

/* GLOBAL VARIABLES */
pthread_t threads[MAX_THREADS];
//pthread_mutex_t mutex;
//pthread_t access;

int pearls = 1000;
double A=0;
double B=0;
double C=0;
double D=0;

/* FUNCTIONS */
void* takePearls(void *arg){
	double ten= 0.10, fifteen= 0.15;

	//take pearls
	while(pearls != 0){
		if(threads[0]){
			printf("Pirate A took: %.2lf pearls...\n", ceil(pearls*ten));
			A += ceil(pearls*ten);
			pearls = pearls - ceil(pearls*ten);
		}
		if(threads[1]){
			printf("Pirate B took: %.2lf pearls...\n", ceil(pearls*ten));
			B += ceil(pearls*ten);
			pearls = pearls - ceil(pearls*ten);
		}
		if(threads[2]){
			printf("Pirate C took: %.2lf pearls...\n", ceil(pearls*fifteen));
			C += ceil(pearls*fifteen);
			pearls = pearls - ceil(pearls*fifteen);
		}
		if(threads[3]){
			printf("Pirate D took: %.2lf pearls...\n", ceil(pearls*fifteen));
			D += ceil(pearls*fifteen);
			pearls = pearls - ceil(pearls*fifteen);
		}
	}
}

void printTotal(double A, double B, double C, double D){

	printf("A: %.2lf Pearls\n", A);
	printf("B: %.2lf Pearls\n", B);
	printf("C: %.2lf Pearls\n", C);
	printf("D: %.2lf Pearls\n", D);
}

int main(void){

	int err;

	pthread_setconcurrency(2);

	int i;
	pthread_create(&(threads[0]), NULL, &takePearls, NULL);
	pthread_create(&(threads[1]), NULL, &takePearls, NULL);
	pthread_create(&(threads[2]), NULL, &takePearls, NULL);
	pthread_create(&(threads[3]), NULL, &takePearls, NULL);
	/*for(i=0; i < MAX_THREADS; i++) {
		err = pthread_create(&(threads[i]), NULL, &takePearls, NULL);
		if (err != 0)
			printf("ERROR: Failed to Create Thread\n");
	}*/

	sleep(1);
	printTotal(A, B, C, D);
	pthread_exit(0);
	return 0;
	
}