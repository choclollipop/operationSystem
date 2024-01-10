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
        perror("access error");
        exit(1);
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

        int bytes = read(fd, buffer, sizeof(buffer) - 1);
        if (bytes < 0)
        {
            perror("read error");
            close(fd);
            _exit(-1);
        }
        
        printf("buffer:%s\n", buffer);
        sleep(4);
    }
    

    return 0;
}