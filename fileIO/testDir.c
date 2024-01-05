#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>

#define BUFFER_SIZE 128

int main()
{
    /* 目录要放置绝对路径 */
    /* 获取绝对路径 */
    char buf[BUFFER_SIZE];
    memset(buf, 0, sizeof(buf));
    getcwd(buf, sizeof(buf) - 1);

    /* 打开当前进程的文件夹 */
    DIR * ptr = opendir(buf);
    if (!ptr)
    {
        perror("open dir error");
    }

    struct stat stabuf;
    stat("./testCommandParam.c", &stabuf);
    printf("stabuf : %d\n\n", stabuf.st_mode);

    struct dirent * pdirent = NULL;

    while ((pdirent = readdir(ptr)) != NULL)
    {
        printf("type: %d\t", pdirent->d_type);
        printf("name : %s\n", pdirent->d_name);
    }



    /* 关闭文件夹 */
    closedir(ptr);

    return 0;
}