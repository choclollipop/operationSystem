#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_SIZE 128

int main()
{
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    char * ptr = getcwd(buffer, sizeof(buffer) - 1);

    printf("buf: %s\n", buffer);
    printf("ptr: %d\n", *ptr);


    return 0;
}