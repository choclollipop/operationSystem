/* 线程拥有独立的栈空间 */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

void * thread_func1(void * arg)
{
    int num = *(int *)arg;
    printf("thread 1 num = %d\n", num);

    num += 100;
    printf("thread 1 num = %d\n", num);

    pthread_exit(NULL);
}

/* 拥有独立栈空间 */
void * thread_func2(void * arg)
{
    int num = *(int *)arg;
    printf("thread 2 num = %d\n", num);

    pthread_exit(NULL);
}

int main()
{
    int num = 100;

    pthread_t tid1,tid2;

    int ret = pthread_create(&tid1, NULL, thread_func1, (void *)&num);
    if (ret != 0)
    {
        printf("pthread_create error\n");
    }

    ret = pthread_create(&tid2, NULL, thread_func2, (void *)&num);
    if (ret != 0)
    {
        printf("pthread_create error\n");
    }

    /* 回收线程资源 */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);


    return 0;
}