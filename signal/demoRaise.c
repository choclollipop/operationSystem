#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

#define SIZE    10

int main()
{
    for (int idx = 0; idx < SIZE; idx++)
    {
        if (idx == 5)
        {
            /* 自己给自己发送终止信号 */
            /* 等价与：kill(getpid(), SIGINT) */
            raise(SIGINT);
        }
        printf("idx = %d\n", idx);
        sleep(1);
    }

    return 0;
}