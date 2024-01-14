#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        _exit(-1);
    }
    if (pid == 0)
    {
        printf("a\n");
        execlp("./main", NULL);
        printf("d\n");
    }
    else
    {
        printf("c\n");
    }

    wait(NULL);
    sleep(30);

    return 0;
}