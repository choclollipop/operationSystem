#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <errno.h>

void signalHandler(int sig)
{
    printf("sig: %d\n", sig);

    printf("有病\n");
}

int main()
{
    /* 注册信号 */
    /* 1.执行默认动作 */
    // signal(SIGINT, SIG_DFL);

    /* 2.忽略 */
    // signal(SIGINT, SIG_IGN);

    /* 3.自定义行为(自定义处理行为) */
    signal(SIGINT, signalHandler);

    while (1)
    {
        printf("hello world\n");
        sleep(2);
    }

    return 0;
}