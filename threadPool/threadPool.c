#include "threadPool.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_MIN_THREADS 5
#define DEFAULT_MAX_THREADS 10
#define DEFAULT_QUEUE_CAPACITY  100
/* 每次扩容/缩容的线程数 */
#define DEFAULT_VARY_THREADS    3

#define TIME_INTERVAL   5

enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    ACCESS_INVAILD,
    UNKNOWN_ERROR,
};

/* 静态函数前置声明 */
static void * threadHander(void * arg);
static void * managerHander(void * arg);
static int threadExitClrResources(threadpool_t *pool);

static int threadExitClrResources(threadpool_t *pool)
{
    for (int idx = 0; idx < pool->maxThreads; idx++)
    {
        if (pool->thredIds[idx] == pthread_self())
        {
            pool->thredIds[idx] = 0;
            break;
        }
    }

    pthread_exit(NULL);
}

/* 本质是一个消费者 */
static void * threadHander(void * arg)
{
    threadpool_t *pool = (threadpool_t *)arg;
    while (1)
    {
        pthread_mutex_lock(&(pool->mutex_pool));
        while (pool->queueSize == 0)
        {
            /* 等待条件变量，生产者发送 */
            pthread_cond_wait(&(pool->notEmpty), &(pool->mutex_pool));

            if (pool->exitNums > 0)
            {
                pool->exitNums--;
                if (pool->liveThreadNums > pool->minThreads)
                {
                    threadExitClrResources(pool);
                }
            }

            if (pool->shoutDown)
            {
                threadExitClrResources(pool);
            }
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

/* 管理者线程 */
static void * managerHander(void * arg)
{
    threadpool_t *pool = (threadpool_t *)arg;
    int flag = 0;

    while (pool->shoutDown == 0)
    {
        sleep(TIME_INTERVAL);

        pthread_mutex_lock(&pool->mutex_pool);
        int taskNum = pool->queueSize;
        int liveNum = pool->liveThreadNums;

        pthread_mutex_unlock(&pool->mutex_pool);

        pthread_mutex_lock(&pool->mutex_busyThread);
        int busyNum = pool->busyThreadNums;
        pthread_mutex_unlock(&pool->mutex_busyThread);

        /* 增加线程数（上限：maxThreads） */
        /* 任务数 > 存活线程数 && 存活线程数 < maxThreads */
        int ret = 0;
        int count = 0;
        if (taskNum > liveNum && liveNum < pool->maxThreads)
        {
            
            /* 一次扩3个 */
            for (int idx = 0; idx < pool->maxThreads && count < DEFAULT_VARY_THREADS && liveNum <= pool->maxThreads; idx++)
            {
                if (pool->thredIds[idx] == 0)
                {
                    ret = pthread_create(&pool->thredIds[idx], NULL, threadHander, pool);
                    if (ret != 0)
                    {
                        perror("thread creat error");
                        break;
                    }
                    count++;
                    pthread_mutex_lock(&pool->mutex_pool);
                    pool->liveThreadNums++;
                    pthread_mutex_unlock(&pool->mutex_pool);
                }
            }
        }

        if (ret != 0)
        {
            flag = 1;
            break;
        }

        /* 减少线程数（下限：minThreads） */
        /* 忙碌线程数 * 2 < 存活线程数  && 存活线程数 > minThreads*/
        if ((busyNum >> 1) < liveNum && liveNum > pool->minThreads)
        {
            pthread_mutex_lock(&pool->mutex_pool);

            pool->exitNums = DEFAULT_VARY_THREADS;
            for (int idx = 0; idx < DEFAULT_VARY_THREADS; idx++)
            {
                pthread_cond_signal(&pool->notEmpty);
            }

            pthread_mutex_unlock(&pool->mutex_pool);
        }
    }

    /* 销毁线程池内所有东西 */
    if (flag)
    {
        threadpoolDestroy(pool);
    }

    if (pool->shoutDown == 1)
    {
        pthread_cancel(pthread_self());
    }
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
        pool->shoutDown = 0;

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

        /* 创建管理者线程 */
        pthread_create(&(pool->managerThread), NULL, managerHander, pool);
        if (ret != 0)
        {
            perror("thread_cread error");
            break;
        }

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

    /* 回收管理者线程资源 */
    pthread_join(pool->managerThread, NULL);

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
    /* 标志位 */
    pool->shoutDown = 1;

    pthread_cond_broadcast(&pool->notEmpty);

    /* 回收资源 */
    pthread_mutex_destroy(&pool->mutex_busyThread);
    pthread_mutex_destroy(&pool->mutex_pool);

    pthread_cond_destroy(&pool->notEmpty);
    pthread_cond_destroy(&pool->notFull);

    if (pool->thredIds)
    {
        free(pool->thredIds);
        pool->thredIds = NULL;
    }

    if (pool->taskQueue)
    {
        free(pool->taskQueue);
        pool->taskQueue = NULL;
    }

    return ON_SUCCESS;
}

