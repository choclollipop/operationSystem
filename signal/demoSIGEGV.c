#include <stdio.h>
#include <string.h>

int main()
{
    char * ptr = NULL;

    strncpy(ptr, "hello world", strlen("hello world"));

    return 0;
}