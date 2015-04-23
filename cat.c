nclude <stdio.h>

const in BUFFER_SIZE = 1024;
int main (int argc, const car *argv[])
{
	char buffer[BUFFER_SIZE];
	char *buf;

	fgets(buffer, BUFFER_SIZE, stdin);
	while (buf != null)
		fputs(buffer, stdout);
		buf = fgets(buffer, BUFFER_SIZE, stdin);
	}
	
	if (feof(stdin)) {
		return 0;}
	else { 
	return 1;
	}
}
