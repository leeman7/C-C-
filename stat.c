/*
*		STAT FILE
*
*
*
*
*/

// LIBRARIES
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <netdb.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdarg.h>

// DEFINES
#define _GNU_SOURCE

// FUNCTIONS
void get_info(struct stat filestat)
{
	// VARIABLES

	// Print info on file
    printf("Info on: \t%s\n", argv[1]);
    printf("--------------------------------------\n");
    printf("OWNER: \t%d\nGROUP: \t%d\n", filestat.st_uid, filestat.st_gid);
    printf("MODE: \t%o\n", filestat.st_mode);
    /* Check
	switch (filestat.st_mode & S_IFMT) {
		case S_IFREG:
			printf("regular file\n");
			break;
		case S_IFDIR:
			printf("directory\n");
			break;
		case S_IFIFO:
			printf("FIFO\n");
			break;
		case S_IFLNK:
			memset(dest, 0, 2014);
			readlink(filestat, dest, 1024);
			if (dest[1024] != 0) {
				fprintf(stderr, "%s: link too long\n", filename);
			} else {
				printf("symbolic link to %s\n", dest);
			}
			break;
	}*/

	if (S_ISREG(filestat.st_mode)) {
		printf("regular file \n");
	}
    printf("DEVICE: %u\n", filestat.st_dev);
    printf("INODE: \t%d\n", filestat.st_ino);
    printf("FILE SIZE: %d Bytes\n", filestat.st_size);
    printf("HARD LINKS: %d\n", filestat.st_nlink);
    printf("SYMBOLIC LINK: %s symbolic link.\n", (S_ISLNK(filestat.st_mode)) ? "is" : "is not");

}

/*void get_directory()
{
	DIR	*dp;
	struct dirent *direntp;

	if ((dir_ptr = opendir(dirname)) == NULL) {
		fprintf(stderr,"ls2: cannot open %s\n", dirname);
	} else {
		while ((direntp = readdir(dir_ptr)) != NULL) {
			if (strcmp(direntp->d_name,".")==0 || strcmp(direntp->d_name,"..")==0) continue;
			statFile( direntp->d_name );
		}
		closedir(dir_ptr);
	}
}*/
void get_permissions(struct stat filestat)
{

	// Get Permissions on file/directory
	printf("FILE PERMISSIONS:   ");
    printf( (S_ISDIR(filestat.st_mode)) ? "d" : "-");
    printf( (filestat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (filestat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (filestat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (filestat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (filestat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (filestat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (filestat.st_mode & S_IROTH) ? "r" : "-");
    printf( (filestat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (filestat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");
}

// MAIN
int main(int argc, char **argv)
{
	// VARIABLES 
	//int flag;
	struct stat filestat;

	// Check the number of arguments passed
	if (argc < 2) {
		return -1;
	}

	// Check if flags were provided
    /*while ((flag=getopt(argc, argv, "a:l:")) >= 0) {
    	switch (flag) {
        	case 'a':
            	break;
        	case 'l':
            	break;
        	case '?':
        	default:
            	fprintf(stderr, "ERROR: Flag not provided\n");
            	exit(EX_USAGE);
    	}
    }*/

    if (stat(argv[1], &filestat) < 0) {
    	//fprintf(stderr, "ERROR: Could not stat %s \n");
    	return -1;
    }

    // Get Standard Info
    get_info(filestat);

    // Get Permissions
    get_permissions(filestat);

    return 0;

}