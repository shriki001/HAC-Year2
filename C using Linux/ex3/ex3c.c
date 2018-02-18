/******************************************************************************
 * Exr. #3c: In this program the father process creates 2 children. each child
 *		    raffle a series of numbers that contain only in range [-1,1].
 *		    each child when raffle 0 - sent it to the father.
 *		    the first children, when raffled -1 sent to his brother; when
 *		    raffled 1 , grows his counter by 1. in simetric way for the 2nd
 *		    child.
 *		    When a process's counter reaches N - he kills the to other and
 *		    finish.
 *
 * =============================================================================
 * Written by  	: Kfir Matityahu & Michael Shriki
 * ID	 	   	: 205425960 / 305599417
 * login	   		: kfirma / michaelshr
 * Course/Year   : OS1 2018
 * Year	   		: B
 *
 * =============================================================================
 * Input: Nothing.
 *
 * -----------------------------------------------------------------------------
 * Output: The number that the 'winner' process counted.
 *
 * =============================================================================
 ******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // for fork()
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <stdbool.h>
// ============================== Defines ================================== //
#define N 10
// ============================== Globals ================================== //
pid_t s[3];        // to keep pid of Father(0), child A(1) ,child B(2) //
// ============================= Prototypes ================================ //
void open_pipes(int pipes[][2]);
void conect_process(pid_t s[], int conect_pipe[]);
bool do_father(int pipes[][2]);
bool do_child_a(int pipes[][2], int num);
bool do_child_b(int pipes[][2], int num);
bool do_child_x(int x, int pipes[][2]);
void can_fork(pid_t process);
// ================================ Main =================================== //
int main() {
	pid_t status;
	int pipes[3][2];
	int conect_pipe[2],i, res = 0;
	open_pipes(pipes);
	if (pipe(conect_pipe) == -1) {
		perror("cannot open pipe");
		exit(EXIT_FAILURE);
	}
	srand((unsigned) time(NULL));
	s[2] = getpid();
	for (i = 0; i < 2; ++i) {
		status = fork();
		can_fork(status);
		if (!status) {
			s[i] = getpid();
			break;
		} else s[i] = status;
	}

	conect_process(s, conect_pipe);
	while (1) {
		if (getpid() == s[0]) {
			res += do_child_x(0, pipes);
			if (res == N) {
				puts("Winner Number = -1");
				kill(s[1], SIGKILL);
				kill(s[2], SIGKILL);
				exit(0);
			}} else if (getpid() == s[1]) {
			res += do_child_x(1, pipes);
			if (res == N) {
				puts("Winner Number = 1");
				kill(s[0], SIGKILL);
				kill(s[2], SIGKILL);
				exit(0);
			}} else if (getpid() == s[2]) {
			res += do_father(pipes);
			if (res == N) {
				puts("Winner Number = 0");
				kill(s[0], SIGKILL);
				kill(s[1], SIGKILL);
				exit(0);
			}}}

	return EXIT_SUCCESS;
}
//---------------------------------------------------------------------------//
// This function share info between processes.
void conect_process(pid_t s[], int conect_pipe[]) {
	int buff[1];
	if (getpid() == s[2]) {
		close(conect_pipe[0]);
		write(conect_pipe[1], &s[1], sizeof(int) * 1);
		close(conect_pipe[1]);
	}if (getpid() == s[0]) {
		close(conect_pipe[1]);
		read(conect_pipe[0], buff, sizeof(int) * N);
		close(conect_pipe[0]);
		s[1] = buff[0];
	}
}
//---------------------------------------------------------------------------//
// This function run the children.
bool do_child_x(int x, int pipes[][2]) {
	int num;
	num = (rand() % 3) - 1;
	if (num == 0) {
		close(pipes[0][0]);
		write(pipes[0][1], &num, sizeof(int) * 1);
		close(pipes[0][1]);
	}
	if (!x)
		return do_child_a(pipes, num);
	else if (x)
		return do_child_b(pipes, num);
	return -1;
}
//---------------------------------------------------------------------------//
// This function do child a.
bool do_child_a(int pipes[][2], int num) {
	if (num < 0)
		return true;
	else {
		close(pipes[2][0]);
		write(pipes[2][1], &num, sizeof(int) * 1);
		close(pipes[2][1]);
	}
	return false;
}
//---------------------------------------------------------------------------//
// This function do child b.
bool do_child_b(int pipes[][2], int num) {
	if (num > 0)
		return true;
	else {
		close(pipes[1][0]);
		write(pipes[1][1], &num, sizeof(int) * 1);
		close(pipes[1][1]);
	}
	return false;
}
//---------------------------------------------------------------------------//
// This function do the father.
bool do_father(int pipes[][2]) {
	int buff[1];
	close(pipes[0][1]);
	read(pipes[0][0], buff, sizeof(int) * N);
	close(pipes[0][0]);
	if (!buff[0])
		return true;
	return false;
}
//---------------------------------------------------------------------------//
// This function gets a processes id. &checks if the program can execute fork()
void can_fork(pid_t process) {
	if (process < 0) {
		perror("Cannot fork!\n");
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------//
// This function gets a pipe
// checks if the program can open the pipe
void open_pipes(int pipes[][2]) {
	int i;
	for (i = 0; i < 3; i++)
		if (pipe(pipes[i]) == -1) {
			perror("cannot open pipe");
			exit(EXIT_FAILURE);
		}
}
