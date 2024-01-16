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
    /* 线程池中的管理线程 */
    pthread_t managerThread;

    /* 最小的线程数 */
    int minThreads;
    /* 最大的线程数 */
    int maxThreads;
    
    int busyThreadNums;
    int liveThreadNums;

    /* 锁和条件变量 */
    pthread_mutex_t mutex_pool;
    /* 维护忙线程的锁 */
    pthread_mutex_t mutex_busyThread;
    /* 任务队列有任务，可以调用线程 */
    pthread_cond_t notEmpty;
    /* 任务队列不满，可以继续存放任务 */
    pthread_cond_t notFull;

    /* 销毁的线程数 */
    int exitNums;

    /* 关闭线程池 */
    int shoutDown;
} threadpool_t;

/* 线程池的初始化 */
int threadpoolInit(threadpool_t *pool, int minThreads, int maxThreads, int queueCapacity);

/* 线程池添加任务 */
int threadPoolAddTask(threadpool_t * pool, void *(*work_hander)(void *), void * arg);

/* 线程池的销毁 */
int threadpoolDestroy(threadpool_t *pool);

#endif //__THREAD_POOL_H_