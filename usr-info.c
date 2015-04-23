/*
*	USER INFORMATION
*
*
*/

// LIBRARIES
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	// user and group id
	uid_t uid;
	gid_t gid;

	struct passwd *pw;
	uid = getuid();
	gid = getgid();

	// get login
	printf("USER IS %s\n", getlogin());

	printf("USER ID: uid= %d, gid= %d\n", uid, gid);

	// get user password
	pw = getpwuid(uid);
	printf("UID passwd entry: \n name= %s, uid= %d, gid= %d, home= %s, shell= %s\n", pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_shell);

	pw = getpwnam("root");
	printf("name=%s, uid=%d, gid=%d, home=%s, shell=%s\n", pw->pw_name, pw->pw_uid, pw->pw_gid, pw->pw_dir, pw->pw_shell);

	exit(0);
}