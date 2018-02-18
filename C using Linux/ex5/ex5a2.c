/******************************************************************************
* Exr. #4a2: This Program used as the APPLICATIONS SERVER.
*            conect to REGISTRY SERVER shared memory, checks the process id-s that 
*		 	 connected to the register server in the memory block
*            this server create a queue , connect to shared memory
*            and allows 2 operations:
*            1) check a primery of a number
*            2) check if a string is palindrome
*            If the process exist on th database, if the result is "true"
*            returns 1, if "false" - returns 0. to add, returns 0.
*            If the process isn't exist on th database, returns -1
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
* =============================================================================
* Input: Nothing.
* -----------------------------------------------------------------------------
* Output: Nothing.
* =============================================================================
******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/shm.h>
// ============================== Defines ================================== //
#define MAX_LEN 30  // size of text message
#define ARR_SIZE 10
// ============================== Structs ================================== //
struct Data {
    pid_t _id;
    char _num[MAX_LEN];
    bool _ans; // used as ans
};
struct msgbuf2 {
    long mtype;
    struct Data _data;
};
// ============================= Prototypes ================================ //
bool is_prime(int n);
bool is_palidrom(char s[]);
void catch_intr(int s);
void connect_shmr(key_t *_key, int *_shmid, char _type);
void open_queue(key_t *_key, char _type);
// ============================== Globals ================================== //
int msqid;
// ================================ Main =================================== //
int main() {
    // declaration on structs for using them
    struct msgbuf2 msg2;
    bool exist = false;
    int shmid, i;
    key_t key,
            key1;
    signal(SIGINT, catch_intr);     //signal handler
    connect_shmr(&key1, &shmid, 'c'); // try connect shared memory of registry
    open_queue(&key, 'b');    // try to open the message queue of apps
    int *shmptr = (int *) shmat(shmid, NULL, SHM_RDONLY);
    if (shmptr == (int *) -1) {
        perror("shmat failed");
        exit(1);
    }
    while (1) {
        if (msgrcv(msqid, &msg2, sizeof(struct Data), 0, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }
        /// ask for registry server to check if the proceess is registered
        /// before he allowed to operate an application
        while (shmptr[0] != 1){ // if register server write to memory block
            for (i = 1; i < ARR_SIZE; i++) {
                if (shmptr[i] == msg2._data._id)
                    exist = true;
            }break;
        }if (!exist)                // process is not registered
            msg2._data._ans = -1;
        else {
            // process if known, therefore is allowed to operate an app
            switch (msg2.mtype) {
                case 1:             // case asked a number
                    msg2._data._ans = is_prime(atoi(msg2._data._num));
                    break;
                case 2:             // case asked a string
                    msg2._data._ans = is_palidrom(msg2._data._num);
                    break;
            }
        }
        msg2.mtype = msg2._data._id;
        // send the message to client
        if (msgsnd(msqid, &msg2, sizeof(struct Data), 0) == -1) {
            perror("msgsnd failed");
            exit(EXIT_FAILURE);
        }
    }
    return EXIT_SUCCESS;
}
// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Signal handler for SIGINT
// close the queue
void catch_intr(int s) {
    if (s == SIGINT)
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("msgctl failed");
            exit(EXIT_FAILURE);
        }exit(EXIT_SUCCESS);
}
//---------------------------------------------------------------------------//
// This function get an integer number
// Return true if it prime number. otherwise - return false.
bool is_prime(int n) {
    int div;         // index for runnig on array
    if (n == 1 || n == 0)
        return false;
    for (div = 2; div <= n / 2; div++)
        if (n % div == 0)
            return false;
    return true;
}
//---------------------------------------------------------------------------//
// This function get a string
// Return true if it a palindrome. otherwise - return false.
bool is_palidrom(char s[]) {
    int i;          // index for runnig on array
    size_t size = strlen(s);   // size of the string
    for (i = 0; i < size / 2; i++)
        if (s[i] != s[size - 1 - i])
            return false;
    return true;
}
//---------------------------------------------------------------------------//
// This function get a key, a message queue id and a char
// Checks if can open the queue using the char as a 'type'
void connect_shmr(key_t *_key, int *_shmid, char _type) {
    if (((*_key) = ftok("/.", _type)) == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }if (((*_shmid) = shmget((*_key), ARR_SIZE, 0600)) == -1) {
        perror("shmget failed");
        exit(EXIT_FAILURE);
    }
}
//---------------------------------------------------------------------------//
// This function get a key, a message queue id and a char
// Checks if can open the queue using the char as a 'type'
void open_queue(key_t *_key, char _type) {
    if (((*_key) = ftok("/.", _type)) == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }if ((msqid = msgget(*_key, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
}
