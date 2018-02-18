#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork()
#include <signal.h>
#include <sys/types.h>
#include <time.h>
//---------------------------------
int count1 = 0, count2 = 0;

void catch_sigusr(int sig_num);
void catch_term(int sig_num);
void do_son(int num);
void do_dad(pid_t son_pid, int num);

int main() {
	int num, i;
	pid_t status;
	signal(SIGUSR1, catch_sigusr);
	signal(SIGUSR2, catch_sigusr);
	signal(SIGTERM, catch_term);

	srand((unsigned) time(NULL));

	status = fork();

	for (i = 0; i < 10; i++) {
		sleep(rand() % 4);

		num = rand() % 2;

		switch (status) {
		case -1:
			perror("fork() failed");
			exit(EXIT_FAILURE);
		case 0:
			do_son(num);
			break;
		default:
			do_dad(status, num);
			break;
		}
	}
	return EXIT_SUCCESS;
}

//---------------------------------
void do_son(int num) {

	if (num == 0) {
		count1++;
		kill(getppid(), SIGUSR1);

	} else {
		count2++;
		kill(getppid(), SIGUSR2);

	}pause();

	if (count1 == 5 || count2 == 5) {
		printf("process %d surrender\n", getpid());
		kill(getppid(), SIGTERM);
		exit(0);
	}
}
//-----------------------------

void do_dad(pid_t son_pid, int num) {
	pause();
	if (num == 0) {
		count1++;
		kill(son_pid, SIGUSR1);

	} else {
		count2++;
		kill(son_pid, SIGUSR2);
	}

	if (count1 == 5 || count2 == 5) {
		printf("process %d surrender\n", getpid());
		kill(son_pid, SIGTERM);
		exit(0);
	}
}

void catch_sigusr(int sig_num) {

	if (sig_num == SIGUSR1)
		printf("process %d caught signal SIGUSR1\n", getpid());

	if (sig_num == SIGUSR2)
		printf("process %d caught signal SIGUSR2\n", getpid());
}

void catch_term(int sig_num) {
	if (sig_num == SIGTERM)
		printf("process %d win\n", getpid());
	exit(0);
}
