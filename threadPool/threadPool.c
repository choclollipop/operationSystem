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

/* 本质是一个消费者 */
void * threadHander(void * arg)
{
    threadpool_t *pool = (threadpool_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&(pool->mutex_pool));
        while (pool->queueSize == 0)
        {
            /* 等待条件变量，生产者发送 */
            pthread_cond_wait(&(pool->notEmpty), &(pool->mutex_pool));
        }

        /* 任务队列一定有任务 */
        task_t tmpTask = pool->taskQueue[pool->queueFront];
        pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
        pool->queueSize--;

        pthread_mutex_unlock(&(pool->mutex_pool));
        pthread_cond_signal(&(pool->notFull));

        pthread_mutex_lock(&(pool->mutex_busyThread));
        pool->busyThreadNums++;
        pthread_mutex_unlock(&(pool->mutex_busyThread));

        /* 执行钩子 */
        tmpTask.work_hander(tmpTask.arg);

        pthread_mutex_lock(&(pool->mutex_busyThread));
        pool->busyThreadNums--;
        pthread_mutex_unlock(&(pool->mutex_busyThread));
    }
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

        pool->busyThreadNums = 0;

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
                ret = pthread_create(&pool->thredIds[idx], NULL, threadHander, (void *)pool);
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

        pool->liveThreadNums = pool->minThreads;

        /* 初始化锁资源 */
        pthread_mutex_init(&(pool->mutex_pool), NULL);
        pthread_mutex_init(&(pool->mutex_busyThread), NULL);

        /* 初始化条件变量 */
        if (pthread_cond_init(&(pool->notEmpty), NULL) != 0 || pthread_cond_init(&(pool->notFull), NULL) != 0)
        {
            perror("thread cond error");
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

    /* 释放锁和条件变量资源 */
    pthread_mutex_destroy(&pool->mutex_busyThread);
    pthread_mutex_destroy(&pool->mutex_pool);

    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    return UNKNOWN_ERROR;
}

/* 线程池添加任务 */
int threadPoolAddTask(threadpool_t * pool, void *(*work_hander)(void *), void * arg)
{
    if (!pool)
    {
        return NULL_PTR;
    }

    pthread_mutex_lock(&pool->mutex_pool);

    while (pool->queueSize == pool->queueCapacity)
    {
        pthread_cond_wait(&pool->notFull, &pool->mutex_pool);
    }

    /* 可以开始放数据 */
    pool->taskQueue[pool->queueRear].work_hander = work_hander;
    pool->taskQueue[pool->queueRear].arg = arg;
    pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
    pool->queueSize++;

    pthread_mutex_unlock(&pool->mutex_pool);
    pthread_cond_signal(&pool->notEmpty);

    return ON_SUCCESS;
}

/* 线程池的销毁 */
int threadpoolDestroy(threadpool_t *pool)
{

}

