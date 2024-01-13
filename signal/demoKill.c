#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SIZE    5

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        _exit(-1);
    }
    else if (pid == 0)
    {
        /* 子进程 */
        printf("I am child process id : %d\n", getpid());

        for (int idx = 0; idx < SIZE; idx++)
        {
            printf("I am child thread\n");
            sleep(1);
        }
    }
    else
    {
        /* 打印PID */
        printf("pid : %d\n", pid);
        /* 父进程 */
        printf("I am parent process id : %d\n", getpid());
        sleep(2);

        printf("kill sub process now\n");
        kill(pid, SIGINT);

    }

    while(1)
    {
        sleep(3);
    }

    return 0;
}