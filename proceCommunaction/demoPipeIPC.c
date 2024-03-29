#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define PIPE_SIZE   2

int main()
{
    int pipefd[PIPE_SIZE];
    memset(pipefd, 0, sizeof(pipefd));

    /* 创建管道:0读，1写 */
    pipe(pipefd);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        _exit(1);
    }

    if (pid == 0)
    {
        // sleep(10);
        /* 关闭管道的读端 */
        close(pipefd[0]);

        /* 子进程 */
        sleep(5);
        int num = 200;
        write(pipefd[1], (void *)&num, sizeof(int));

        // int ReadNum = 0;
        // read(pipefd[0], (void *)&ReadNum, sizeof(int));
        // printf("parent process : ReadNum:%d\n", ReadNum);


        /* 关闭写端 */
        close(pipefd[1]);
    }
    else if (pid > 0)
    {
        /* 关闭管道的写端 */
        close(pipefd[1]);
        // close(pipefd[0]);

        /* 父进程 */
        

        /* 把read设置为非阻塞 */
        int flag = fcntl(pipefd[0], F_GETFL);
        flag |= O_NONBLOCK;
        fcntl(pipefd[0], F_SETFL, flag);

        int readNum = 0;
        read(pipefd[0], (void *)&readNum, sizeof(int));
        printf("parent process : readNum:%d\n", readNum);


        /* 关闭读端 */
        close(pipefd[0]);
        _exit(0);
    }

    /* 阻塞等待回收子进程资源 */
    wait(NULL);

    // while (1)
    // {
    //     sleep(5);
    //     printf("hello world\n");
    // } 

    return 0;
}