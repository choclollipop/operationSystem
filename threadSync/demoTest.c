#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

pthread_mutex_t g_mutex;

void printer(char * str)
{
    pthread_mutex_lock(&g_mutex);
    while (*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;
        usleep(100);
    }
    pthread_mutex_unlock(&g_mutex);
    printf("\n");
}

void * thread_func1(void * arg)
{
    char *str = "hello";
    printer(str);
}

void * thread_func2(void * arg)
{
    char *str = "world";
    printer(str);
}

int main()
{
    /* 给锁默认的‘fast’属性 */
    pthread_mutex_init(&g_mutex, NULL);

    pthread_t tid1,tid2;

    int ret = pthread_create(&tid1, NULL, thread_func1, NULL);
    if (ret != 0)
    {
        printf("pthread_create error\n");
    }

    ret = pthread_create(&tid2, NULL, thread_func2, NULL);
    if (ret != 0)
    {
        printf("pthread_create error\n");
    }

    /* 回收线程资源 */
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    // pthread_mutex_lock(&g_mutex);

    /* 释放锁的资源:必须解锁后才可以销毁锁 */
    ret = pthread_mutex_destroy(&g_mutex);
    if (ret == EBUSY)
    {
        printf("pthread_mutex_destroy error\n");
        _exit(-1);
    }

    return 0;
}