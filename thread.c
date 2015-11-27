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
int pearls = 1000;

struct pirate{
  pthread_t pirate_id;
  char password[20];
  int myPearls;
};

/* FUNCTIONS */
void * takePearls(void *arg){
	double ten= 0.10, fifteen= 0.15;
	struct pirate *p = arg;
	//take pearls

		if(strcmp(p->password, "Open Sesame")==0){
			printf("Pirate A took: %.2lf pearls...\n", ceil(pearls*ten));
			p->myPearls += ceil(pearls*ten);
			pearls = pearls - ceil(pearls*ten);
		}
		if(strcmp(p->password, "Open Watermelon")==0){
			printf("Pirate C took: %.2lf pearls...\n", ceil(pearls*fifteen));
			p->myPearls += ceil(pearls*fifteen);
			pearls = pearls - ceil(pearls*fifteen);
		}
 
  return p;
}

void showPirateBooty(double A, double B, double C, double D){
	printf("Pirate A Grabbed: %.2lf Pearls\n", A);
	printf("Pirate B Grabbed: %.2lf Pearls\n", B);
	printf("Pirate C Grabbed: %.2lf Pearls\n", C);
	printf("Pirate D Grabbed: %.2lf Pearls\n", D);
}

int main(void){
  pthread_t threads[MAX_THREADS];
	int err, i;
  struct pirate *pirates; /* pointer to struct info */
  
  /* */
  pirates = calloc(MAX_THREADS, sizeof(struct pirate));
	
	for(i=0; i < MAX_THREADS; i++){
    pirates[i].myPearls=0;
    if(i <= 1){
      strcpy(pirates[i].password, "Open Sesame");
		}
    else{
      strcpy(pirates[i].password, "Open Watermelon");
    }
    
    err = pthread_create(&(threads[i]), NULL, &takePearls, &pirates[i]);
		if (err != 0)
			printf("ERROR: Failed to Create Thread\n");
	}
 
  for(i=0; i < MAX_THREADS; i++){
		pthread_join(threads[i], NULL);
	}

	sleep(100); /* add in a pause */
 
	showPirateBooty(pirates[0].myPearls, pirates[1].myPearls, pirates[2].myPearls, pirates[3].myPearls);
 
  free(pirates);
	pthread_exit(0);
	return 0;
	
}