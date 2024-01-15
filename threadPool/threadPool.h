#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include <pthread.h>

/* 线程池结构体 */
typedef struct threadpool_t
{
    /* 线程池中的线程id */
    pthread_t * thredIds;
    /* 最小的线程数 */
    int minThreads;
    /* 最大的线程数 */
    int maxThreads;
} threadpool_t;

/* 线程池的初始化 */
int threadpoolInit(threadpool_t *pool, int minThreads, int maxThreads);


/* 线程池的销毁 */
int threadpoolDestroy(threadpool_t *pool);

#endif //__THREAD_POOL_H_