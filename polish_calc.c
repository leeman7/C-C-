/*
*	POLISH CALCULATOR
*
*
*/

// LIBRARIES
#include <stdio.h>
#include <stdlib.h>

// STATIC VARIABLES
#define SIZE 100
#define NUMBER '0'

// FUNCTION DEFINITIONS
int getop(char []);
void push(double);
double pop(void);

// MAIN
int main(argc, *argv[])
{
	int type;
	double operand;
	char buffer[SIZE]

	while((type = getop(buffer)) != EOF) {
		switch (type) {
			case NUMBER:
				push(atof(buffer));
				break;
			case '+':
				push(pop() + pop())
				break;
			case '*':
				push(pop() * pop());
				break;
			case '-':
				operand = pop();
				push(pop() - operand);
				break;
			case '/':
				operand = pop();
				if (operand != 0.0)
						push(pop() / operand);
				else
					printf("ERROR: CANNOT DIVIDE BY 0\n");
				break;
			case '\n':
				printf("\t%.8g\n", pop());
				break;
			default:
				printf("ERROR: UNKOWN COMMAND%s\n", buffer);
				break;
		}	
	}

	return 0;
}

// FUNCTIONS
void push(double var) 
{
	int sp = 0; // next free stack position
	double value[SIZE];

	if (sp < SIZE)
		value[sp++] = var;
	else
		printf("ERROR: STACK IS FULL, CANNOT PUSH %g\n", var);
}

double pop(void)
{
	int sp = 0;

	if (sp > 0) {
		return value[--sp];}
	else {
		printf("ERROR: STACK IS EMP\n");
		return 0.0;
	}
}
