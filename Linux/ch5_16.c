#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

/* 기본 예제: utmp/wtmp 파일의 모든 레코드 출력 */
int main(int argc, char *argv[])
{
    int fd;
    struct utmp ut;
    char buf[32];

    if (argc < 2) {
        fprintf(stderr, "사용법: %s <utmp|wtmp 파일>\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    while (read(fd, &ut, sizeof(ut)) == sizeof(ut)) {
        time_t t = ut.ut_tv.tv_sec;
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&t));
        printf("type=%d pid=%d line=%-12.12s user=%-10.32s host=%-16.256s %s\n",
               ut.ut_type, ut.ut_pid, ut.ut_line, ut.ut_user, ut.ut_host, buf);
    }
    close(fd);
    return 0;
}
