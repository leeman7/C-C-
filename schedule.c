/*
 *Project 4   

You are to write a program that takes a reference string and the number
of frames as inputs. Suppose the maximum length of a reference string is 30 and
the maximum number of frames is 8. Find out the different number of page faults 
using Optimal, LRU, Most Frequently used, and Least frequently used 
algorithms. If there is a tie, use FIFO algorithm to break the tie.
 *
 */

/* LIBRARIES */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* FUNCTIONS */
int LRU(int frame, char * refstr, char * pages){
	eint faults = 0, counter[frame], i, count;
	int j, k;

	//initialize counter array with 0's
	for(k=0; k < frame; k++)
		counter[k] = 0;

	count = 0;
	int mfu = 0;
	for(i=0; i < strlen(refstr); i++){
		int pos = (i % frame);

		//check if page is empty
		if(pages[pos] == -1){
			pages[pos] = refstr[i];
			faults++;

		} else { //if table is filled with tables
			//check whole page table
			for (j = 0; j < frame; j++){
			//check if frame exists in page table
				if(mfu < counter[j])
					mfu = counter[j];
				if(pages[j] == refstr[i]){
					counter[j] += 1; //most frequent counter
					printf("Page: %d -- Ref: %d\n", j, i);
					printf("Counter: %d\n", counter[j]);
					count=0;
				} else { //if page does not exist bring it in replace mfu
					if (pages[j] != refstr[i] && counter[j] == mfu){ //if the counter at j is equal to mfu value then replace it
						pages[j] = refstr[i];
						faults++;
					}
				}

			}
		}
	}

	return faults;

}
int OPT(int frame, char * refstr, char * pages){
	iint faults = 0, counter[frame], i, count;
	int j, k;

	//initialize counter array with 0's
	for(k=0; k < frame; k++)
		counter[k] = 0;

	int mfu = 0;
	for(i=0; i < strlen(refstr); i++){
		int pos = (i % frame);

		//check if page is empty
		if(pages[pos] == -1){
			pages[pos] = refstr[i];
			faults++;

		} else { //if table is filled with tables
			//check whole page table
			for (j = 0; j < frame; j++){
			//check if frame exists in page table
				if(mfu < counter[j]){
					mfu = counter[j];
					printf("changed: %d\n", mfu);
				}
				if(pages[j] == refstr[i]){
					counter[j] += 1; //most frequent counter
					printf("Page: %d -- Ref: %d\n", j, i);
					printf("Counter: %d\n", counter[j]);
				} else { //if page does not exist bring it in replace mfu
					if (pages[j] != refstr[i] && counter[j] == mfu){ //if the counter at j is equal to mfu value then replace it
						pages[j] = refstr[i];
						faults++;
					}
				}

			}
		}
	}

	return faults/2;
}

int LFU(int frame, char * refstr, char * pages){
	int faults = 0, i;

	for(i=0; i < strlen(refstr); i++){
		int current = (i % frame);
		if(pages[current] != refstr[i]){
			pages[current] = refstr[i];
			faults++;
		}
	}

	return faults;
}
int MFU(int frame, char * refstr, char * pages){
	int faults = 0, counter[frame], i, count;
	int j, k;

	//initialize counter array with 0's
	for(k=0; k < frame; k++)
		counter[k] = 0;

	int mfu = 0;
	for(i=0; i < strlen(refstr); i++){
		int pos = (i % frame);

		//check if page is empty
		if(pages[pos] == -1){
			pages[pos] = refstr[i];
			faults++;

		} else { //if table is filled with tables
			//check whole page table
			for (j = 0; j < frame; j++){
			//check if frame exists in page table
				if(mfu < counter[j]){
					mfu = counter[j];
					printf("changed: %d\n", mfu);
				}
				if(pages[j] == refstr[i]){
					counter[j] += 1; //most frequent counter
					printf("Page: %d -- Ref: %d\n", j, i);
					printf("Counter: %d\n", counter[j]);
				} else { //if page does not exist bring it in replace mfu
					if (pages[j] != refstr[i] && counter[j] == mfu){ //if the counter at j is equal to mfu value then replace it
						pages[j] = refstr[i];
						faults++;
					}
				}

			}
		}
	}

	return faults/2;
}

/* MAIN */
int main(int argc, char * argv[]){

	int i, frame;
	char refstr[31];
	int lru, mfu;

	if(argc != 3)
		printf("ERROR: Invalid arguments\n");
	else {
		if(atoi(argv[2]) < 0 || atoi(argv[2]) > 8)
			printf("ERROR: Invalid Frame Size\n");
		else{
			frame = atoi(argv[2]);
		}

		char pages[frame];
		for (i = 0; i < frame; ++i)
			pages[i] = -1;

		if(strlen(argv[1]) < 30)
			strcpy(refstr, argv[1]);
		else
			printf("ERROR: Invalid Refererence String\n");


		lru=LRU(frame, refstr, pages);
		mfu=MFU(frame, refstr, pages);
		lfu=LFU(frame, refstr, pages);
		opt=OPT(frame, refstr, pages);
		printf("LRU: %d\n", lru);
		printf("MFU: %d\n", mfu);
		printf("LFU: %d\n", lru);
		printf("OPT: %d\n", mfu);
	}

	return 0;
}