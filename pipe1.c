/* first pipe example, a pipe to the process itself */
#include <unistd.h>
#include <stdio.h>

// this figure includes terminating null
#define MSGSIZE 16

char *msg1= "hello, world #1";
char *msg2= "hello, world #2";
char *msg3= "hello, world #3";

main()
{
 char inbuf[MSGSIZE];
 int p[2], j;
 
 if (pipe(p) == -1)
 {
   perror("pipe call");
   exit(1);
 }
 
 // write down pipe
 write(p[1], msg1, MSGSIZE);
 write(p[1], msg2, MSGSIZE);
 write(p[1], msg3, MSGSIZE);
 
 // read from pipe
 for(j=0; j<3; j++)
 {
   read(p[0], inbuf, MSGSIZE);
   printf("%s\n", inbuf);
 }
 
 exit(0);
 }