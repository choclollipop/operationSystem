#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/mman.h>

int main()
{
    int fd = open("./mmap.txt", O_RDWR | O_CREAT, 0644);
    if (fd == -1)
    {
        perror("open error");
        _exit(-1);
    }

    void * ptr = mmap(NULL, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        perror("mmap error");
        close(fd);
        _exit(-1);
    }

    close(fd);

    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork error");
        _exit(-1);
    }

    if (pid == 0)
    {
        sleep(3);
        /* 子进程 */
        printf("%s", (char *)ptr);
    }
    if (pid > 0)
    {
        /* 父进程写 */
        strcpy((char *)ptr, "hello child\n");
    }


#if 0
    /* 打印映射区内容 */
    printf("ptr:%s", (char *)ptr);

    strcpy((char *)ptr, "666");

    printf("ptr:%s\n", (char *)ptr);
#endif 

    /* 回收子进程 */
    wait(NULL);

    printf("end\n");

    /* 关闭映射 */
    int ret = munmap(ptr, 1024);
    if (ret == -1)
    {
        perror("munmap error");
        _exit(-1);
    }

    return 0;
}