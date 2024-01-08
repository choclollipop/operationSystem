#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int num = 0;

    /* fork()函数的返回值，在子进程中返回0，在父进程中返回pid，返回-1则表示进程创建失败 */
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        return 0;
    }
    if (pid == 0)
    {
        /* 休眠1s，让出cpu，目的：让父进程的逻辑先执行完 */
        sleep(1);
        printf("nums:%d\n", num);

        /* 子进程中 */
        pid_t childPid = getid();
        printf("childPid:%d\n", childPid);

        pid_t parentPid = getppid();
        printf("parentPid process:%d\n", parentPid);
        
    }
    else if (pid > 0)
    {
        printf("nums:%d\n", num);
        /* 此时是父进程 */
        pid_t parentPid = getid();
        printf("parentPid process:%d\n", parentPid);

        num = 400;
        printf("nums:%d\n", num);
    }

    return 0;
}