#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

void * thread_fun (void * arg)
{
    pthread_detach(pthread_self());

    while (1)
    {
        printf("I am thread ......\n");
        sleep(1);
    }
    printf("hello world\n");

    pthread_exit(NULL);
}

int main()
{
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, thread_fun, NULL);
    if (ret != 0)
    {
        printf("thread crete error");
        _exit(1);
    }

    sleep(3);
    pthread_cancel(tid);

    return 0;
}