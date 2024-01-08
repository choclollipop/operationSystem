#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    /* 获取当前进程的pid */
    pid_t curpid = getpid();

    /* 获取当前进程的父进程的pid */
    pid_t parentpid = getppid();

    printf("curpid: %d\n", curpid);
    printf("parentpid: %d\n", parentpid);


    while (1)
    {
        printf("hello world\n");

        sleep(5);
    }

    return 0;
}