#ifndef __THREAD_POOL_H_
#define __THREAD_POOL_H_

#include <pthread.h>

typedef struct task_t
{
    /* 线程的回调函数 */
    void *(*work_hander)(void *arg);
    /* 参数 */
    void * arg;
} task_t;


/* 线程池结构体 */
typedef struct threadpool_t
{
    /* 任务队列 */
    task_t * taskQueue;
    int queueCapacity;
    /* 任务队列中的任务数 */
    int queueSize;
    /* 任务队列的头尾指针 */
    int queueFront;
    int queueRear;

    /* 线程池中的线程id */
    pthread_t * thredIds;
    /* 最小的线程数 */
    int minThreads;
    /* 最大的线程数 */
    int maxThreads;
} threadpool_t;

/* 线程池的初始化 */
int threadpoolInit(threadpool_t *pool, int minThreads, int maxThreads, int queueCapacity);


/* 线程池的销毁 */
int threadpoolDestroy(threadpool_t *pool);

#endif //__THREAD_POOL_H_