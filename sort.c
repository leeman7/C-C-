/*****************************************************
 *		OPERATING SYSTEMS
 *
 * Date: Sun Sep 20 14:45:46 CDT 2015 
 * Author: Lee Nardo 
 * Class: Operating Systems
 * Project: #1.2
 * File: sort.c
 * Summary:  
 *  
 *****************************************************/

// LIBRARIES
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER 1028

// Create struct for strings
struct StringNode {
	char data[BUFFER];
	struct StringNode * next;
};

// Destructor for Sting linked list
void destroyList(struct StringNode * root) {
	
	// Check if at head
	if(root == NULL) {
		free(root);
	} else if(root->next != NULL) {
		destroyList(root->next);
		free(root);
	}
}

// Sort List
void sortedInsert(struct StringNode * head, char * str) {
 	
 	// Variables
 	struct StringNode * current = head;
 	int set = 1;

 	if(strcmp(current->data, "") == 0) {
 		strcpy(current->data, str);
 	} else {
 		while(set == 1) {
 			int comp = strcmp(current->data, str);

 			if(comp == 0) {
 				struct StringNode * newNode = malloc(sizeof(struct StringNode));
 				strcpy(newNode->data, str);
 				newNode->next = current->next;
 				current->next = newNode;	
 				set = 0;
 			} else if(comp > 0) {
 				struct StringNode * newNode = malloc(sizeof(struct StringNode));
 				strcpy(newNode->data, current->data);
 				memset(current->data, 0, BUFFER);
 				strcpy(current->data, str);
 				newNode->next = current->next;
 				current->next = newNode;
 				set = 0;
 			} else {
 				if(current->next == NULL){
 					struct StringNode * newNode = malloc(sizeof(struct StringNode));
 					strcpy(newNode->data, str);
 					newNode->next = NULL;
 					current->next = newNode;
 					set = 0;
 				} else {
 					current = current->next;
 				}
 			}
 		}
 	}
}

// Print our list of strings
void printList(struct StringNode * cursor) {
 	while(cursor != NULL) {
 		printf("%s", cursor->data);
 		cursor = cursor->next;
 	}
}

// Main
int main(int argc, char * argv[]) {

	// Variables
	struct StringNode * list = malloc(sizeof(struct StringNode));
	struct StringNode * current = list;
	char buf[BUFFER];
	list->next = NULL;

	// Continue to sort list
	while(fgets(buf, BUFFER, stdin) != NULL){
		sortedInsert(list, buf);
	}

	printf("\n");
	// Print list
	printList(current);
	// deallocate memory and destroy our list
	destroyList(list);
	return 0;
}