#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

typedef struct stuInfo
{
    int age;
    char sex;
} stuInfo;

int g_retVal = 1;

void * thread_func(void *arg)
{
    stuInfo *stu = (stuInfo *)arg;

    printf("stu.age:%d, stu:sex:%c\n", stu->age, stu->sex);
    printf("i am new thread\n");

    pthread_t tid = pthread_self();
    printf("thread tid : %ld\n", tid);

    // sleep(10);
    /* 栈空间 离开函数后地址被释放了 */
    // int retVal = 1;

    pthread_exit(&g_retVal);
}

int main()
{
    pthread_t tid;

    stuInfo stu;
    stu.age = 10;
    stu.sex = 'f';

    int num = 100;
    /* 线程创建 */
    int ret = pthread_create(&tid, NULL, thread_func, (void *)&stu);
    if (ret != 0)
    {
        char * error = strerror(ret);
        perror(error);
        // printf("error:%s\n", error);
        _exit(1);
    }

    printf("man tid:%ld\n", tid);

    int * reVal = NULL;
    ret = pthread_join(tid, (void **)&reVal);
    if (ret != 0)
    {
        perror("pthread_join error");
        _exit(1);
    }

    printf("reVal:%d\n", *reVal);

    printf("ret : %d\n", ret);
    printf("hello world\n");

    usleep(300);
    return 0;
}