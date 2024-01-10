#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

void * thread_func(void * arg)
{
    pthread_detach(pthread_self());

    printf("thread fun.....\n");

    sleep(10);
}

/* 线程分离 */
int main()
{
    pthread_t tid;
    /* 创建线程 */
    int ret = pthread_create(&tid, NULL, thread_func, NULL);
    if (ret != 0)
    {
        char * error = strerror(ret);
        perror(error);
        _exit(1);
    }

    sleep(1);
    /* 等待回收资源 */
    ret = pthread_join(tid, NULL);
    if (ret != 0)
    {
        printf("ret:%d\n", ret);
        _exit(-1);
    }

    sleep(1);
    return 0;
}