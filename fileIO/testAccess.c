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
    /* access函数用来判断文件是否存在 */
    int ret = access("./heiheihei.c", F_OK);
    if (ret != 0)
    {
        perror("access error");
    }
    

    return 0;
}