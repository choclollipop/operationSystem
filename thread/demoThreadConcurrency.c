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

void * thread_func1(void *arg)
{
    stuInfo *stu = (stuInfo *)arg;

    printf("stu.age:%d, stu.sex:%c\n", stu->age, stu->sex);


    pthread_exit(NULL);
}

void * thread_func2(void *arg)
{
    int val = *(int *)arg;
    printf("thrad2 val:%d\n", val);

    pthread_exit(NULL);
}

int main()
{
    pthread_t tid1;
    pthread_t tid2;

    stuInfo stu;
    stu.age = 10;
    stu.sex = 'f';

    /* 线程创建 */
    int ret = pthread_create(&tid1, NULL, thread_func1, (void *)&stu);
    if (ret != 0)
    {
        char * error = strerror(ret);
        perror(error);
        // printf("error:%s\n", error);
        _exit(1);
    }

    int num = 100;
    ret = pthread_create(&tid2, NULL, thread_func2, (void *)&num);
    if (ret != 0)
    {
        char * error = strerror(ret);
        perror(error);
        // printf("error:%s\n", error);
        _exit(1);
    }



    ret = pthread_join(tid1, NULL);
    if (ret != 0)
    {
        perror("pthread_join error");
        _exit(1);
    }

    ret = pthread_join(tid2, NULL);
    if (ret != 0)
    {
        perror("pthread_join error");
        _exit(1);
    }


    usleep(300);
    return 0;
}