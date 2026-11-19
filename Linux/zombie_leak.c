/* zombie_leak.c */

#include <stdio.h>
#include <unistd.h>

int main (void) {
	for (;;) {
		pid_t pid = fork();
		if (pid < 0) { perror("fork"); pause(); }
		if (pid == 0)  _exit(0); /* 즉시 종료 */
		usleep(10000);	         /* 0.01초 */
	}
}
