#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <semaphore.h>

sem_t g_sem;

void printer(char * str)
{
    sem_wait(&g_sem);
    while (*str != '\0')
    {
        putchar(*str);
        fflush(stdout);
        str++;
        usleep(100);
    }
    sem_post(&g_sem);
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
    sem_init(&g_sem, 0, 1);

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
    ret = sem_destroy(&g_sem);

    return 0;
}