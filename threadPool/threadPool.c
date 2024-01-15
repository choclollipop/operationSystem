#include "threadPool.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define DEFAULT_MIN_THREADS 5
#define DEFAULT_MAX_THREADS 10

enum STATUS_CODE
{
    ON_SUCCESS,
    NULL_PTR,
    MALLOC_ERROR,
    ACCESS_INVAILD,
    UNKONW_ERROR,
};

void * threadHander(void * arg)
{

    pthread_exit(NULL);
}

/* 线程池的初始化 */
int threadpoolInit(threadpool_t *pool, int minThreads, int maxThreads)
{
    if (!pool)
    {
        return NULL_PTR;
    }

    do 
    {
        /* 判断合法性 */
        if (minThreads < 0 || maxThreads < 0 || minThreads >= maxThreads)
        {
            maxThreads = DEFAULT_MAX_THREADS;
            minThreads = DEFAULT_MIN_THREADS;
        }
        /* 更新线程池属性 */
        pool->maxThreads = maxThreads;
        pool->minThreads = minThreads;

        /* 为线程池中的线程id分配存放空间 */
        pool->thredIds = (pthread_t *)malloc(sizeof(pthread_t) * maxThreads);
        if (!pool->thredIds)
        {
            perror("malloc error");
            break;
        }
        memset(pool->thredIds, 0, sizeof(pthread_t) * maxThreads);

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

        return ON_SUCCESS;

    } while(0);

    /* 程序到这里，上面一定有失败 */
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

    return UNKONW_ERROR;
}

/* 线程池的销毁 */
int threadpoolDestroy(threadpool_t *pool)
{

}

