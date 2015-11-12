/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Mon Oct 12 15:32:11 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #3
 * File: thread_b.c
 * Summary: This is part B of Project 3 where a mutex
 *  lock is used to block access to our resources.
 *  Once again the program generates threads to pull
 *  Pearls away, but this time only one pirate can 
 *  access the Pearls at a time.
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
pthread_mutex_t lock;

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
			pthread_mutex_lock(&lock);
			printf("Pirate A took: %.2lf pearls...\n", ceil(pearls*ten));
			A += ceil(pearls*ten);
			pearls = pearls - ceil(pearls*ten);
			pthread_mutex_unlock(&lock);
		}
		if(threads[1]){
			pthread_mutex_lock(&lock);
			printf("Pirate B took: %.2lf pearls...\n", ceil(pearls*ten));
			B += ceil(pearls*ten);
			pearls = pearls - ceil(pearls*ten);
			pthread_mutex_unlock(&lock);
		}
		sleep(2);
	}
}

void * takePearlsCD(void *arg){
	double fifteen= 0.15;

	while(pearls > 0){
		if(threads[2]){
			pthread_mutex_lock(&lock);
			printf("Pirate C took: %.2lf pearls...\n", ceil(pearls*fifteen));
			C += ceil(pearls*fifteen);
			pearls = pearls - ceil(pearls*fifteen);
			pthread_mutex_unlock(&lock);
		}
		if(threads[3]){
			pthread_mutex_lock(&lock);
			printf("Pirate D took: %.2lf pearls...\n", ceil(pearls*fifteen));
			D += ceil(pearls*fifteen);
			pearls = pearls - ceil(pearls*fifteen);
			pthread_mutex_unlock(&lock);
		}
		sleep(1);
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

	pthread_setconcurrency(4);

	if(pthread_mutex_init(&lock, NULL) != 0){
		printf("ERROR: Failed to create Lock\n");
		return 1;
	}

	pthread_create(&(threads[0]), NULL, &takePearlsAB, NULL);
	pthread_create(&(threads[1]), NULL, &takePearlsAB, NULL);
	pthread_create(&(threads[2]), NULL, &takePearlsCD, NULL);
	pthread_create(&(threads[3]), NULL, &takePearlsCD, NULL);

	sleep(10);

	printTotal(A, B, C, D);

	pthread_mutex_destroy(&lock);
	pthread_exit(0);

	return 0;	

}