#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    int ret = truncate("./hello.txt", 20);
    if (ret == -1)
    {
        perror("truncate error");
    }

    return 0;
}