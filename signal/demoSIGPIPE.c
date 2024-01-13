#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PIPE_SIZE   2

/* 向没有读端的管道写数据:使用GDB可以查看到该信号 */
int main()
{
    int pipefd[PIPE_SIZE];
    memset(pipefd, 0, sizeof(pipefd));

    /* 创建无名管道 */
    pipe(pipefd);

    /* 关闭读端 */
    close(pipefd[0]);

    sleep(1);

    int num = 100;
    int ret = write(pipefd[1], (void *)&num, sizeof(int));
    if (ret < 0)
    {
        perror("write error\n");
        _exit(-1);
    }
    sleep(10);

    close(pipefd[1]);

    return 0;
}