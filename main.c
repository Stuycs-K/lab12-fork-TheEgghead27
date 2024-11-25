#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void child(int seedOffset, int * time) {
	srand(seedOffset + getpid());
	*time = rand() % 5 + 1;
	printf("%d %dsec\n", getpid(), *time);
	sleep(*time);
	printf("%d finished after %dsec\n", getpid(), *time);
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
	srand( time(NULL) );
	int seedOffset = rand();  // because I didn't feel like using raw PID
	pid_t pids[2];
	int slps[2];
	for (int i = 0; i < 2; i++) {
		pids[i] = verified_fork();
		if (pids[i] == 0) {
			child(seedOffset, slps+i);
			return 0;
		}
	}
	return 0;
}
