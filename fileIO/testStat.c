#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUFFER_SIZE   32

void get_file_time(__time_t st_time)
{
    struct tm* info = localtime(&(st_time));
    printf("%02d %02d %02d:%02d\n", info->tm_mon + 1, info->tm_mday, info->tm_hour, info->tm_min);

}

int main()
{
    struct stat bufStat;

    stat("./hello.txt", &bufStat);

    // printf("mode : %d\n", bufStat.st_mode);
    // printf("mode : %ld\n", bufStat.st_size);

    // printf("mode : %ld\n", bufStat.st_atime);
    // printf("mode : %ld\n", bufStat.st_mtime);
    // printf("mode : %ld\n", bufStat.st_ctime);

    FILE * pfd = popen("md5sum testStat.c", "r");
    if(pfd == NULL)
    {
        perror("popen error");
    }

    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    return 0;
}