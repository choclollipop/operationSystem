#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE     1024

int main()
{
    int fd = open("./hello.txt", O_RDWR);
    if (fd < 0)
    {
        perror("open error");
    }
    
    char buffer[BUFFER_SIZE] = {0};
    /* 读文件 */
    read(fd, buffer, sizeof(buffer) - 1);
    printf("buffer: %s\n", buffer);

    close(fd);

    return 0;
}