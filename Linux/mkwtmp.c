#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

/* 테스트용 wtmp 생성: 로그인(USER_PROCESS)/로그아웃(DEAD_PROCESS) 기록 */
static void put(int fd, short type, int pid, const char *line,
                const char *user, time_t t)
{
    struct utmp ut;
    memset(&ut, 0, sizeof(ut));
    ut.ut_type = type;
    ut.ut_pid = pid;
    strncpy(ut.ut_line, line, sizeof(ut.ut_line));
    strncpy(ut.ut_user, user, sizeof(ut.ut_user));
    ut.ut_tv.tv_sec = t;
    write(fd, &ut, sizeof(ut));
}

int main(void)
{
    int fd = open("test_wtmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    struct tm tm = {0};
    tm.tm_year = 126; tm.tm_mon = 9; tm.tm_mday = 20; tm.tm_hour = 9;
    tm.tm_isdst = -1;
    time_t base = mktime(&tm);

    put(fd, USER_PROCESS, 1001, "pts/0", "ubuntu", base);
    put(fd, USER_PROCESS, 1002, "pts/1", "alice",  base + 600);
    put(fd, DEAD_PROCESS, 1001, "pts/0", "",       base + 2 * 3600 + 30 * 60);
    put(fd, DEAD_PROCESS, 1002, "pts/1", "",       base + 4 * 3600 + 5 * 60);
    put(fd, USER_PROCESS, 1003, "pts/0", "ubuntu", base + 5 * 3600);
    close(fd);
    return 0;
}
