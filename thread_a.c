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
#define ITER 11

/* GLOBAL VARIABLES */
pthread_t threads[MAX_THREADS];

int pearls = 1000;
double A=0;
double B=0;
double C=0;
double D=0;

/* FUNCTIONS */
void * takePearlsAB(void *arg){
	double ten= 0.10;

	//take pearls
	while(pearls > 0){
		
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
	}
}

void * takePearlsCD(void *arg){
	double fifteen= 0.15;

	while(pearls > 0){

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
		//sleep(1);
	}

}

void printTotal(double A, double B, double C, double D){

	int sum = 0;
	sum = A+B+C+D;

	printf("\nPirate A: %.2lf Pearls\n", A);
	printf("Pirate B: %.2lf Pearls\n", B);
	printf("Pirate C: %.2lf Pearls\n", C);
	printf("Pirate D: %.2lf Pearls\n", D);
	printf("Amount of Pearls: %d\n", sum);
}

int main(void){

	pthread_create(&(threads[0]), NULL, &takePearlsAB, NULL);
	pthread_create(&(threads[1]), NULL, &takePearlsAB, NULL);
	pthread_create(&(threads[2]), NULL, &takePearlsCD, NULL);
	pthread_create(&(threads[3]), NULL, &takePearlsCD, NULL);

	sleep(3);
	printTotal(A, B, C, D);
	pthread_exit(0);

	return 0;
	
}