#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE   20

int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        printf("argc param must be 3");
    }

    int fd = open(argv[1], O_RDWR | O_CREAT, 0644);
    if (fd < 1)
    {
        perror("open error");
    }

    int copyfd = open(argv[2], O_RDWR | O_CREAT, 0644);
    if (copyfd < 1)
    {
        perror("open error");
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    ssize_t bytes = 0;

    while (bytes)
    {
        bytes = read(fd, 0, sizeof(buffer) - 1);
        if(bytes == 0)
        {
            break;
        }
        write(copyfd, buffer, bytes);
        if(bytes < sizeof(buffer) - 1)
        {
            break;
        }
    }

    close(fd);
    close(copyfd);

    return 0;
}