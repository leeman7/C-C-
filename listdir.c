/*
*	list all directories
*
*
*/

// LIBRARIES
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void printdir(char *dir, int depth)
{
	DIR *dp; // directory pointer
	struct dirent *entry; 
	struct stat statbuf; // stat buffer

	//if pointer to directory does not exist
	//return an error message
	if ((dp = opendir(dir)) == NULL) {
		fprintf(stderr, "ERROR: COULD NOT OPEN DIRECTORY - %s\n", dir);
		return;
	}

	// change directories
	chdir(dir);
	while((entry = readdir(dp)) != NULL) {
		lstat(entry->d_name, &statbuf);
		if (S_ISDIR(statbuf.st_mode)) {
			/* FOUND A DIRECTORY BUT IGNORE . and .. */
			if (strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0) continue;
			printf("%*s%s/\n", depth, "", entry->d_name);
			/* RECURSE  AT A NEW INDENT LEVEL */
			printdir(entry->d_name, depth+6);
		} else	printf("%*s%s\n", depth, "", entry->d_name);
	}
	// change directories down into sub-directories
	chdir("..");
	closedir(dp); // close the directory using the pont to the dir
}

// Main
int main()
{
	printf("Directory scan of /home: \n");
	printdir("/home", 0);
	printf("Done scanning. \n");

	return 0;
}
