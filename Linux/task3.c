#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

/* 과제 3: USER_PROCESS와 같은 ut_line의 DEAD_PROCESS를 짝지어 접속 시간 계산
   사용법: ./task3 /var/log/wtmp  (last 명령의 (시:분)과 비교) */
int main(int argc, char *argv[])
{
    int fd, n = 0, cap = 1024, count = 0;
    struct utmp *rec = malloc(cap * sizeof(struct utmp));
    char in[32], out[32];
    const char *path = argc > 1 ? argv[1] : "/var/log/wtmp";

    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    while (read(fd, &rec[n], sizeof(struct utmp)) == sizeof(struct utmp)) {
        if (++n == cap)
            rec = realloc(rec, (cap *= 2) * sizeof(struct utmp));
    }
    close(fd);

    printf("=== 접속 시간 계산 (USER_PROCESS + DEAD_PROCESS) ===\n");
    for (int i = 0; i < n; i++) {
        if (rec[i].ut_type != USER_PROCESS)
            continue;
        time_t login = rec[i].ut_tv.tv_sec;
        strftime(in, sizeof(in), "%m-%d %H:%M", localtime(&login));

        int j;
        for (j = i + 1; j < n; j++) {
            if (strncmp(rec[j].ut_line, rec[i].ut_line, sizeof(rec[i].ut_line)) != 0)
                continue;
            if (rec[j].ut_type == DEAD_PROCESS)
                break;
            if (rec[j].ut_type == USER_PROCESS) {   /* 짝 없이 새 로그인 */
                j = n;
                break;
            }
        }
        printf("사용자: %-8.32s 터미널: %-8.32s 로그인: %s ",
               rec[i].ut_user, rec[i].ut_line, in);
        if (j < n) {
            time_t logout = rec[j].ut_tv.tv_sec;
            long sec = (long)(logout - login);
            strftime(out, sizeof(out), "%H:%M", localtime(&logout));
            printf("로그아웃: %s 접속시간: (%02ld:%02ld)\n",
                   out, sec / 3600, (sec % 3600) / 60);
        } else {
            printf("로그아웃: 기록 없음\n");
        }
        count++;
    }
    printf("검색된 기록: %d개\n", count);
    free(rec);
    return 0;
}
