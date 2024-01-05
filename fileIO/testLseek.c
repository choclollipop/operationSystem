#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>

int main()
{

#if 1

    /* 使用lseek来计算文件大小 */
    int fd = open("./hello.txt", O_RDWR);
    if (fd < 0)
    {
        perror("open error");
    }

    off_t offset = lseek(fd, 3, SEEK_SET);

    char *ptr = "1,2 ,3\n";
    write(fd, ptr, strlen(ptr));


    // off_t offset = lseek(fd, 0, SEEK_END);
    // printf("offset: %ld\n", offset);

    close(fd);

#endif

    return 0;
}