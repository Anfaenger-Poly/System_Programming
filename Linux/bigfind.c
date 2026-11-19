#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <dirent.h>
#include <sys/stat.h>

static double min_bytes;
static long total_files, big_files;
static double big_bytes;

static void walk(const char *dir)
{
    DIR *dp = opendir(dir);
    if (dp == NULL) {
        perror(dir);
        return;
    }

    struct dirent *e;
    errno = 0;
    while ((e = readdir(dp)) != NULL) {
        if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0)
            continue;

        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);

        struct stat st;
        if (lstat(path, &st) == -1) {
            perror(path);
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            walk(path);
        } else if (S_ISREG(st.st_mode)) {
            total_files++;
            if ((double)st.st_size >= min_bytes) {
                big_files++;
                big_bytes += (double)st.st_size;
                printf("%10.2f MB  %s\n", st.st_size / 1048576.0, path);
            }
        }
        errno = 0;
    }
    if (errno != 0)
        perror(dir);

    if (closedir(dp) == -1)
        perror(dir);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "사용법: %s <디렉토리> <기준MB>\n", argv[0]);
        return 1;
    }

    min_bytes = atof(argv[2]) * 1048576.0;
    printf("[bigfind] %s 아래 %s MB 이상 파일 검색\n", argv[1], argv[2]);
    walk(argv[1]);
    printf("-- 검사 %ld개 중 기준 이상 %ld개, 합계 %.2f MB --\n",
           total_files, big_files, big_bytes / 1048576.0);
    return 0;
}
