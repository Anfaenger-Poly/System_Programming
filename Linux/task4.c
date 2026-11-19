#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <utmp.h>

/* 과제 4(심화): default에서 EMPTY, INIT_PROCESS도 출력하고 타입별 개수 확인 */
static const char *name(int t)
{
    switch (t) {
    case EMPTY:         return "EMPTY";
    case RUN_LVL:       return "RUN_LVL";
    case BOOT_TIME:     return "BOOT_TIME";
    case NEW_TIME:      return "NEW_TIME";
    case OLD_TIME:      return "OLD_TIME";
    case INIT_PROCESS:  return "INIT_PROCESS";
    case LOGIN_PROCESS: return "LOGIN_PROCESS";
    case USER_PROCESS:  return "USER_PROCESS";
    case DEAD_PROCESS:  return "DEAD_PROCESS";
    case ACCOUNTING:    return "ACCOUNTING";
    default:            return "UNKNOWN";
    }
}

int main(int argc, char *argv[])
{
    int fd, cnt[16] = {0};
    struct utmp ut;
    char buf[32];
    const char *path = argc > 1 ? argv[1] : "/var/run/utmp";

    if ((fd = open(path, O_RDONLY)) == -1) {
        perror("open");
        exit(1);
    }
    while (read(fd, &ut, sizeof(ut)) == sizeof(ut)) {
        time_t t = ut.ut_tv.tv_sec;
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&t));
        if (ut.ut_type >= 0 && ut.ut_type < 16)
            cnt[ut.ut_type]++;
        printf("%-14s pid=%-6d line=%-12.32s user=%-10.32s %s\n",
               name(ut.ut_type), ut.ut_pid, ut.ut_line, ut.ut_user, buf);
    }
    close(fd);
    printf("\n--- 타입별 개수 ---\n");
    for (int i = 0; i < 10; i++)
        printf("%-14s %d\n", name(i), cnt[i]);
    return 0;
}
