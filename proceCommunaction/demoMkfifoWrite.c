#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 32

/* 创建有名管道 */
int main()
{
    int ret = access("./testfifo", F_OK);
    if (ret == -1)
    {
        ret = mkfifo("./testfifo", 0644);
        if (ret == -1)
        {
            perror("mkfifo error");
            _exit(1);
        }
    }

    int fd = open("./testfifo", O_RDWR);
    if (fd == -1)
    {
        perror("open error");
        _exit(-1);
    } 

    char buffer[BUFFER_SIZE];
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        strncpy(buffer, "hello world", sizeof(buffer) - 1);

        write(fd, buffer, strlen(buffer) - 1);

        sleep(3);
    }
    
    

    return 0;
}