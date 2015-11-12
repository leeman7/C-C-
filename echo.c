#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int echo();

int main() {

    echo();
    return 0;
}

int echo() {
    char input[512];

    while(fgets(input, 512, stdin)){
        printf("%s\n", input);
        memset(input, 0, strlen(input));
    }

    return 1;
}