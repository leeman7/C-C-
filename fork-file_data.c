// This program shows how files are handled across forks
// assume "data" is at least 20 chars long

#include <fcntl.h>
#include <unistd.h>

main()
{
  int fd;
  pid_t pid;
  char buf[10];

  if ((fd = open("data", O_RDONLY)) == -1)
    perror("open failed");

  read(fd, buf, 10); // advance file pointer

  printpos("Before fork", fd);

  switch(pid = fork()) {
  case -1:
    perror("fork failed");
    break;

  case 0: 
    printpos("Child before read", fd);
    read(fd, buf, 10);
    printpos("Child after read", fd);
    break;

  default:
    wait((int *)0);
    printpos("Parent after wait", fd);
  }
}

// print position in file
int printpos(const char *string, int filedes)
{
  off_t pos;
  
  if ((pos = lseek(filedes, 0, SEEK_CUR)) == -1)
    perror("lseek failed");
  printf("%s:%ld\n", string, pos);
}


/* output:
Before fork:10
Child before read:10
Child after read:20
Parent after wait:20
*/