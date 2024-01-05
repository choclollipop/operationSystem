#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>

int main()
{

#if 0
    /* 打开文件但不创建 */
    int fd = open("./hello.txt", O_RDWR, 0644);
    if(fd == -1)
    {
        perror("open error:");
    }
#endif

#if 1

    /* 打开文件若没有就创建 */
    int fd1 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd1 == -1)
    {
        perror("open error");
    }
    printf("fd1 = %d\n", fd1);

    int fd2 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd2 == -1)
    {
        perror("open error");
    }
    printf("fd2 = %d\n", fd2);

    int fd3 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd3 == -1)
    {
        perror("open error");
    }
    printf("fd3 = %d\n", fd3);

    int fd4 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd4 == -1)
    {
        perror("open error");
    }
    printf("fd4 = %d\n", fd4);

    /* 关闭文件 */
    close(fd1);

    int fd5 = open("./hello.txt", O_RDWR | O_CREAT, 0644);
    if (fd5 == -1)
    {
        perror("open error");
    }
    printf("fd5 = %d\n", fd5);

    close(fd2);
    close(fd3);
    close(fd4);
    close(fd5);



#endif

    return 0;
}