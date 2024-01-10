#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int main()
{
    /* 获取调用线程的ID号 */
    pthread_t tid = pthread_self();
    printf("tid:%ld\n", tid);

    /* 获取此时的进程号 */
    pid_t pid = getpid();
    printf("pid:%d\n", pid);

    

    return 0;
}