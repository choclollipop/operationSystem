#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    int num = 100;

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        _exit(1);
    }

    if (pid == 0)
    {
        /* 子进程 */
        printf("child process : num = %d\n", num);
#if 0
        /* 实现进程通信：方式一：写入文件内 */
        int fd = open("./test.txt", O_RDWR | O_CREAT, 0644);

        num = num + 100;
        write(fd, &num, sizeof(int));

        /* 关闭文件 */
        close(fd);
#endif

        
        printf("child process : num = %d\n", num);
    }
    else if (pid > 0)
    {
        /* 父进程 */
        printf("parent process : num = %d\n", num);
        sleep(1);
#if 0
        int fd = open("./test.txt", O_RDWR);
        read(fd, &num, sizeof(int));

        /* 关闭文件 */
        close(fd);
#endif

        printf("parent process : num = %d\n", num);

    }

    /* 阻塞等待回收子进程资源 */
    wait(NULL);

    printf("hello world\n");

    return 0;
}