#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

/* 과제 2: 사용자 이름을 인자로 받아 그 사용자의 기록만 출력
   사용법: ./task2 /var/log/wtmp ubuntu */
int main(int argc, char *argv[])
{
    int fd, count = 0;
    struct utmp ut;
    char buf[32];

    if (argc < 3) {
        fprintf(stderr, "사용법: %s <파일> <사용자>\n", argv[0]);
        exit(1);
    }
    if ((fd = open(argv[1], O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    printf("=== %s 사용자의 로그인 기록 ===\n", argv[2]);
    while (read(fd, &ut, sizeof(ut)) == sizeof(ut)) {
        if (strncmp(ut.ut_user, argv[2], sizeof(ut.ut_user)) != 0)
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
