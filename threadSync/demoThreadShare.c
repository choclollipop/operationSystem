#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

/* 余量 */
int g_margin = 10;
pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

/* 线程共享全局变量 */

void * thread_func1(void * arg)
{
    // printf("thread 1 num = %d\n", g_margin);

    while (1)
    {
        // usleep(300);
        pthread_mutex_lock(&g_mutex);
        if (g_margin > 0)
        {
            usleep(100);
            g_margin -= 1;
        }
        pthread_mutex_unlock(&g_mutex);

        printf("thread 1 num = %d\n", g_margin);
        if (g_margin <= 0)
        {
            break;
        }
    }

    printf("thread 1 num = %d\n", g_margin);


    pthread_exit(NULL);
}

void * thread_func2(void * arg)
{
    usleep(50);
    while (1)
    {
        pthread_mutex_lock(&g_mutex);
        if (g_margin >= 2)
        {
            usleep(300);
            g_margin -= 2;
        }
        pthread_mutex_unlock(&g_mutex);
        
        printf("thread 2 num = %d\n", g_margin);
        // usleep(200);
        if (g_margin <= 0)
        {
            break;
        }
    }

    printf("thread 2 num = %d\n", g_margin);

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

    /* 释放锁 */
    pthread_mutex_destroy(&g_mutex);


    return 0;
}