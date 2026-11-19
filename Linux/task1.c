#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

/* 과제 1: USER_PROCESS만 출력 (who 명령과 비교)
   사용법: ./task1 [utmp 파일] */
int main(int argc, char *argv[])
{
    int fd, count = 0;
    struct utmp ut;
    char buf[32];
    const char *path = argc > 1 ? argv[1] : "/var/run/utmp";

    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    printf("=== 현재 로그인 사용자 (USER_PROCESS) ===\n");
    while (read(fd, &ut, sizeof(ut)) == sizeof(ut)) {
        if (ut.ut_type != USER_PROCESS)
            continue;
        time_t t = ut.ut_tv.tv_sec;
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&t));
        printf("사용자: %-10.32s 터미널: %-10.32s 시간: %s\n",
               ut.ut_user, ut.ut_line, buf);
        count++;
    }
    printf("검색된 기록: %d개\n", count);
    close(fd);
    return 0;
}
