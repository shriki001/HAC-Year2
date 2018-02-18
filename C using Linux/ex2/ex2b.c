#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for fork()
#include <signal.h>
#include <sys/types.h>
#include <time.h>

//---------------------------------

void catch_sigusr(int sig_num); 
void catch_alarm(int sig_num);
void do_func(pid_t pid);

int main() {
	int num;
	pid_t status, dad = getpid();

	signal(SIGUSR1, catch_sigusr); // definde signal hendlers
	signal(SIGALRM, catch_alarm);

	srand((unsigned) time(NULL));

	status = fork();

	while (1) {
		num = rand() % 10;

		if (num < 3) {
			printf("process %d end\n", getpid());
			exit(EXIT_SUCCESS);
		} else {

			switch (status) {
			case -1:
				perror("fork() failed");
				exit(EXIT_FAILURE);
			case 0:
				do_func(dad);
				break;
			default:
				do_func(status);
				break;
			}
		}
	}

	return EXIT_SUCCESS;
}

//-----------------------------

// function that get process ID and send a signal to the other process
// after that this process go to "sleep" for 5 sec
void do_func(pid_t pid) {

	kill(pid, SIGUSR1);
	alarm(5);
	pause();
	alarm(0);

}
// signal hendler for the timer
 
void catch_alarm(int sig_num) {
	printf("process %d was left alone, and quits\n", getpid());
	exit(EXIT_SUCCESS);
}
// signal hendler for the sending signal to another process 
void catch_sigusr(int sig_num) {
printf("process %d has a partner\n", getpid());
}
