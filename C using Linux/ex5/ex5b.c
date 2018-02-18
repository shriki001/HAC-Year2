/******************************************************************************
* Exr. #5b: Father process allocate shared memory and produces 2 children
*           processes. Each one of the children raffles on loop a number until
*           he raffles a prime number.
*           When doing so (he raffled a prime number):
*           he stores him at the shared memory.
*           kill his 'brother' (the other child process).
*           sends a signal to (his) father process and finish.
*           The father process views the number - and also finished.
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   	    : kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
* =============================================================================
* Input: Nothing.
* -----------------------------------------------------------------------------
* Output: The prime number one of the child (succeeded) to raffle.
* =============================================================================
******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <time.h>
#include <unistd.h>
// ============================== Defines ================================== //
#define ARR_SIZE    5          // size of the pid array
#define MAX_RAFFLE 1000       // the range (limit) of the raffled numbers
// ============================= Prototypes ================================ //
bool is_prime(int num);
void open_shmr(key_t *_key, int *_shmid, char _type);
void catch_sigusr(int sig_num);
void close_shm(int shmid);
void do_child(int *shmptr);
void can_fork(pid_t process);
// ================================ Main =================================== //
int main() {
    int shmid,          // 1st shared memory , keeps the prime number
            index;          // index for running on array
    int  *shmptr;      // pointer to the 1st shared memory
    pid_t status;       // keep the current process id
    key_t shm_key;     // keep key of the shared memory
    // try to open the shred memory,
    open_shmr(&shm_key, &shmid, 'a');// try to open the shred memory,
    // Creating a pointer to memory block
    shmptr = (pid_t *) shmat(shmid, NULL, 0);
    if (shmptr == (int *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    signal(SIGUSR1, catch_sigusr); // define a signal handler
    srand((unsigned) time(NULL));
    shmptr[4] = getpid();        // father's process id is kept on place[2]
    // father process produces 2 children
    for (index = 0; index < 2; index++) {
        status = fork();
        can_fork(status);           // check if fork not failed
        if (!status) {
            shmptr[index + 2] = getpid(); // son's process id is kept on place[i]
            // first on [2], second on [3]
            break;
        } else
            shmptr[index + 2] = status;        // father process
    }
    if (getpid() != shmptr[4])     // case this process is a child process
        do_child(shmptr);
    pause();      // father waits for signal from finish child
    printf("the prime num is: %d\n", shmptr[1]);
    close_shm(shmid); // closing all the shared memory the program used
    return EXIT_SUCCESS;
}
// ============================= Functions ================================= //
// This function pointer to shared memory
// Performs (generally) each of the children process method
void do_child(int *shmptr) {
    int num;        // keeps the number that the process raffled
    while (1) {
        num = rand() % MAX_RAFFLE;
        if (is_prime(num)){      // if we raffled a prime number
            while (shmptr[0] != 1) { // if some process write to memory block
                shmptr[0] = 1;
                shmptr[1] = num;
                shmptr[0] = 0;
                break;
            }if (getpid() == shmptr[2]) {       // case is the 1st child
                kill(shmptr[3], SIGKILL);      // kills his brother
                printf("son:(a) %d  killed brother:(b) %d\n",
                       shmptr[2], shmptr[3]);
            } else if (getpid() == shmptr[3]) {      // case is the 2nd child
                kill(shmptr[2], SIGKILL);          // kills his brother
                printf("son:(b) %d  killed brother:(a) %d\n",
                       shmptr[3], shmptr[2]);
            }kill(shmptr[4], SIGUSR1);
            exit(EXIT_SUCCESS);
        }}
}
//---------------------------------------------------------------------------//
// This function get an id of shared memory
// Closes the the shared memory
void close_shm(int shmid) {
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        exit(EXIT_FAILURE);
    }
}
//---------------------------------------------------------------------------//
// This function get a key, a shared memory id and a char
// Checks if can open the shared memory using the char as a 'type'
void open_shmr(key_t *_key, int *_shmid, char _type) {
    if (((*_key) = ftok("/.", _type)) == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }if (((*_shmid) = shmget(*_key, ARR_SIZE, 0600 | IPC_CREAT | IPC_EXCL)) ==
        -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
}
//---------------------------------------------------------------------------//
// signal handler for catching end of process (user defined signal)
void catch_sigusr(int sig_num) {
    if (sig_num == SIGUSR1)
        printf("father process (%d) caught signal SIGUSR1\n", getpid());
}
//---------------------------------------------------------------------------//
// This function gets an integer number
// Return true if it prime number. otherwise - return false.
bool is_prime(int num) {
    int div;         // index for running on array
    if (num == 1 || num == 0)       // 0 and 1 are not primes
        return false;
    for (div = 2; div <= num / 2; div++)
        if (num % div == 0)
            return false;
    return true;
}
//---------------------------------------------------------------------------//
// This function gets a processes id.
// checks if the program can execute fork()
void can_fork(pid_t process) {
    if (process < 0) {
        perror("Cannot fork!\n");
        exit(EXIT_FAILURE);
    }
}