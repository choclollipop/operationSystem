#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    /* 更改名字后再次执行，会报错，因为源文件名称被更改，第二次编译执行，认为没有a.out源文件 */
    int ret = rename("./a.out", "./main");
    if (ret == -1)
    {
        perror("rename error");
    }

    return 0;
}