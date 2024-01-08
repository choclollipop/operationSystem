#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <error.h>
#include <wait.h>

int main()
{

#if 0
    /* 当进程没有子进程时 */
    printf("hello world\n");
    wait(NULL);
    printf("hello world\n");
#endif

#if 1
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
        sleep(1);

        int childState = 1;
        _exit(childState);
    }
    else if (pid > 0)
    {
        /* 此时是父进程 */
        pid_t parentPid = getpid();
        printf("parentPid process:%d, num: %d\n", parentPid, num);
    }

    /* 看此时是否阻塞 */
    int status = 0;
    pid_t waitPid = wait(&status);

    int ret = WEXITSTATUS(status);

    printf("waitPid: %d\n", waitPid);
    printf("status: %d\n", status);
    printf("ret: %d\n", ret);


    printf("hello world\n");

#endif 

    return 0;
}