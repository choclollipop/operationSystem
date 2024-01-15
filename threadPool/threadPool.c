#include "threadPool.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_MIN_THREADS 5
#define DEFAULT_MAX_THREADS 10
#define DEFAULT_QUEUE_CAPACITY  100

enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    ACCESS_INVAILD,
    UNKNOWN_ERROR,
};

void * threadHander(void * arg)
{

    pthread_exit(NULL);
}

/* 线程池的初始化 */
int threadpoolInit(threadpool_t *pool, int minThreads, int maxThreads, int queueCapacity)
{
    if (!pool)
    {
        return NULL_PTR;
    }

    do 
    {
        /* 判断合法性 */
        if (minThreads <= 0 || maxThreads <= 0 || minThreads >= maxThreads)
        {
            maxThreads = DEFAULT_MAX_THREADS;
            minThreads = DEFAULT_MIN_THREADS;
        }
        /* 更新线程池属性 */
        pool->maxThreads = maxThreads;
        pool->minThreads = minThreads;

        if (queueCapacity <= 0)
        {
            queueCapacity = DEFAULT_QUEUE_CAPACITY;
        }
        pool->queueCapacity = queueCapacity;
        pool->queueFront = 0;
        pool->queueRear = 0;
        pool->queueSize = 0;

        pool->taskQueue = (task_t *)malloc(sizeof(task_t) * pool->queueCapacity);
        if (!pool->taskQueue)
        {
            perror("malloc error");
            break;
        }
        memset(pool->taskQueue, 0, sizeof(task_t) * pool->queueCapacity);

        /* 为线程池中的线程id分配存放空间 */
        pool->thredIds = (pthread_t *)malloc(sizeof(pthread_t) * pool->maxThreads);
        if (!pool->thredIds)
        {
            perror("malloc error");
            break;
        }
        memset(pool->thredIds, 0, sizeof(pthread_t) * pool->maxThreads);

        int ret = 0;
        /* 创建线程 */
        for (int idx = 0; idx < minThreads; idx++)
        {
            if (pool->thredIds[idx] == 0)
            {
                ret = pthread_create(&pool->thredIds[idx], NULL, threadHander, NULL);
                if (ret != 0)
                {
                    perror("pthread_create error");
                    break;
                }
            } 
        }
        /* 创建线程函数的返回值，若不为0，即此时创建的线程出错 */
        if (ret != 0)
        {
            break;
        }

        return ON_SUCCESS;

    } while(0);

    /* 程序到这里，上面一定有失败 */

    /* 回收任务队列的堆空间 */
    if (pool->taskQueue)
    {
        free(pool->taskQueue);
        pool->taskQueue = NULL;
    }

    /* 回收线程资源 */
    for (int idx = 0; idx < minThreads; idx++)
    {
        if (pool->thredIds[idx])
        {
            pthread_join(pool->thredIds[idx], NULL);
        }
    }

    if (pool->thredIds != NULL)
    {
        free(pool->thredIds);
        pool->thredIds = NULL;
    }

    return UNKNOWN_ERROR;
}

/* 线程池的销毁 */
int threadpoolDestroy(threadpool_t *pool)
{

}

