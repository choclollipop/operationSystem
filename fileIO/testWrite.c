#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>

int main()
{

#if 0
    int fd = open("./hello.txt", O_RDWR);
    if (fd == -1)
    {
        perror("open error");
    }

    /* 向文件中写入数据 */
    char * ptr = "hello world\n";
    int bytes = write(fd, ptr, strlen(ptr));
    if (bytes < 0)
    {
        perror("write error");
    }
    else
    {
        printf("bytes : %d\n", bytes);
    }

    close(fd);
#endif

#if 0
    /* O_TRUNC表示清空文件中的内容 */
    int fd = open("./hello.txt", O_RDWR | O_TRUNC);
    if (fd == -1)
    {
        perror("open error");
    }

    /* 关闭文件 */
    close(fd);
#endif

#if 0
    /* 以追加的方式打开文件 */
    int fd = open("./hello.txt", O_RDWR | O_APPEND);
    if (fd == -1)
    {
        perror("open error");
    }

    char * ptr = "1, 2, 3\n";
    write(fd, ptr, strlen(ptr));

    close(fd);
#endif

    return 0;
}