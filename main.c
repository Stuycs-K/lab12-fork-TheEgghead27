#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

void child(int seedOffset) {
	srand(seedOffset + getpid());
	int time = rand() % 5 + 1;
	printf("%d %dsec\n", getpid(), time);
	sleep(time);
	printf("%d finished after %dsec\n", getpid(), time);
	exit(time);
}

pid_t verified_fork() {
	pid_t ret = fork();
	if (ret < 0) {
		fprintf(stderr, "fork: %s\n", strerror(errno));
		exit(errno);
	}
	return ret;
}

int main(void) {
	const int n = 2;
	srand( time(NULL) );
	int seedOffset = rand();  // because I didn't feel like using raw PID
	pid_t parent = getpid();
	int duration;
	printf("%d about to create %d child processes\n", parent, n);
	for (int i = 0; i < n; i++) {
		if (!verified_fork()) {
			child(seedOffset);
			return 255;  // should never happen
		}
	}
	int buf;
	pid_t child = wait(&buf);
	if (child < 0) {
		fprintf(stderr, "wait: %s\n", strerror(errno));
		return errno;
	}
	if (!WIFEXITED(buf)) {
		fprintf(stderr, "child did not exit normally!\n");
		return 1;
	}
	printf("Main Process %d is done. Child %d slept for %dsec\n", parent, child, WEXITSTATUS(buf));
	return 0;
}
