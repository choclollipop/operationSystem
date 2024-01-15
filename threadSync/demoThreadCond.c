#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

/* 锁 */
pthread_mutex_t g_mutex;

/* 条件变量 */
pthread_cond_t  g_notEmpty;  /* 生产者给消费者 */
pthread_cond_t  g_notFull;   /* 消费者给生产者 */

/* 链表结点 */
typedef struct Node
{
    int val;
    struct Node * next;
} Node;

/* 链表的头指针 */
Node* head = NULL;

/* 生产者消费者模型 */

/* 生产者 */
void * produceFunc(void * arg)
{
    int count = 1;
    while (1)
    {
        Node * newNode = (Node *)malloc(sizeof(Node) * 1);
        if (!newNode)
        {
            perror("malloc error");
            pthread_exit(NULL);
        }
        memset(newNode, 0, sizeof(Node) * 1);
        newNode->val = count++;

        /* 加锁 */
        pthread_mutex_lock(&g_mutex);

        /* 头插 */
        newNode->next = head;
        head = newNode;

        printf("===produce:%ld\t%d\n", pthread_self(), newNode->val);
        pthread_mutex_unlock(&g_mutex);
        
        /* 信号通知 */
        pthread_cond_signal(&g_notEmpty);

        /* 休眠1~3s */
        sleep(100);

    }

    /* 线程退出 */
    pthread_exit(NULL);
}

/* 消费者 */
void * consumeFunc(void * arg)
{
    while (1)
    {
        /* 等待非空的条件变量 */
        pthread_mutex_lock(&g_mutex);

        /* 当没有数据的时候就条件等待 */
        while (!head)
        {
            pthread_cond_wait(&g_notEmpty, &g_mutex);
        }

        /* 消费数据 */
        Node * delNode = head;
        head = head->next;

        printf("=====consume%ld\t%d\n", pthread_self(), delNode->val);

        if (delNode)
        {
            free(delNode);
            delNode = NULL;
        }

        pthread_mutex_unlock(&g_mutex);

        usleep(200);   
    }
    
    pthread_exit(NULL);
}


int main()
{
    pthread_t produce_tid, consume_tid;

    /* 初始化锁和条件变量 */
    pthread_mutex_init(&g_mutex, NULL);
    pthread_cond_init(&g_notEmpty, NULL);
    pthread_cond_init(&g_notFull, NULL);

    /* 生产者 */
    int ret = pthread_create(&produce_tid, NULL, (void *)produceFunc, NULL);
    if (ret != 0)
    {
        printf("pthread_create error\n");
        _exit(-1);
    }

    /* 消费者 */
    ret = pthread_create(&consume_tid, NULL, (void *)consumeFunc, NULL);
    if (ret != 0)
    {
        printf("pthread_create error\n");
        _exit(-1);
    }

    /* 回收线程资源 */
    pthread_join(produce_tid, NULL);
    pthread_join(consume_tid, NULL);

    /* 释放锁和条件变量资源 */
    pthread_mutex_destroy(&g_mutex);
    pthread_cond_destroy(&g_notEmpty);
    pthread_cond_destroy(&g_notFull);

    return 0;
}