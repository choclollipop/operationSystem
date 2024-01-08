#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <error.h>
#include <wait.h>

int main()
{
    int num = 100;

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        _exit(0);
    }
    if (pid == 0)
    {
        /* 子进程中 */
        pid_t childPid = getpid();
        printf("childPid:%d, num: %d\n", childPid, num);


        /* 子进程不结束，延时两分钟 */
        while (1)
        {
            pid_t parentPid = getppid();
            printf("childProcess parentPidd:%d\n", parentPid);
            sleep(2);
        }

        pid_t parentPid = getppid();
        printf("childProcess parentPidd:%d\n", parentPid);
        int childState = 1;
        _exit(childState);
    }
    if (pid > 0)
    {
        /* 此时是父进程 */
        pid_t parentPid = getpid();
        printf("parentPid process:%d, num: %d\n", parentPid, num);
    }

    /* 看此时是否阻塞 */
    // int status = 0;
    // pid_t waitPid = wait(&status);

    // int ret = WEXITSTATUS(status);

    // printf("waitPid: %d\n", waitPid);
    // printf("status: %d\n", status);
    // printf("ret: %d\n", ret);

    printf("hello world\n");

    return 0;
}