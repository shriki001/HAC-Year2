/******************************************************************************
* Exr. #4a1: This Program used as the REGISTRY SERVER.
* 			 this server create a shared memory to storage an process id-s array            
*            and create a shared memory and a queue, and allows 3 operations:
*            1) add a new process
*            2) check if a process if registered on system
*            3) removal of existing process
*            when get code:
*            1: if successed to add, return 0.
*               if not - return 1 because it alredy exist, and return 2
*               because it not exist at all.
*            2: if exist -return 1, else -return 0.
*            3: just remove from the database
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   	: kfirma / michaelshr
* Course/Year  : OS1 2018
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
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/shm.h>
// ============================== Defines ================================== //
#define MAX_LEN 30                  // size of text message
#define ARR_SIZE 10                 // the size of the pid array
// ============================== Structs ================================== //
struct msgbuf1 {
    long mtype;
    char m_text[MAX_LEN];
};
// ============================= Prototypes ================================ //
void catch_intr(int s);
int search_pid(pid_t wanted);
int exist_pid(pid_t wanted);
void open_queue(key_t *_key, int *_msqid, char _type);
void open_shmr(key_t *_key, int *_shmid, char _type);
// ============================== Globals ================================== //
int msqid;
int shmid;
int *shmptr;
// ================================ Main =================================== //
int main() {
    struct msgbuf1 msg;
    bool flag = false;        // flag for checking if we still run
    key_t msq_key;            // key of message queue of this server (registry)
    key_t shm_key;            // key of shared memory of this server (registry)
    int process_id;                  // keep process's id
    int first_empty;                // indicate first cell is empty
    int is_exist;                   // indicate if process exist
    signal(SIGINT, catch_intr);     //  signal handler
    open_queue(&msq_key, &msqid, 'a');      // try to open the message queue
    open_shmr(&shm_key, &shmid, 'c');       // try to open the shred memory
    // Creating a ptr to memory block
    shmptr = (pid_t *) shmat(shmid, NULL, 0);
    if (shmptr == (int *) -1) {
        perror("shmat failed");
        exit(EXIT_FAILURE);
    }
    // the main loop of the running server
    while (1) {
        if (msgrcv(msqid, &msg, MAX_LEN, 0, 0) == -1) {
            perror("msgrcv failed");
            exit(EXIT_FAILURE);
        }process_id = atoi(msg.m_text);
        shmptr[0] = 1;      //this server write to the memory
        switch (msg.mtype) {
            case 1:              // -- adding a process -- //
                flag = false;
                first_empty = search_pid(process_id);
                if (first_empty == -1)              // process is found
                    strcpy(msg.m_text, "0 | CANT ADD (process is exist!)");
                else{        // process is not found
                    if (first_empty ==
                        ARR_SIZE)        // dont have an empty place
                        strcpy(msg.m_text, "2 | CANT ADD (array is FULL!)");
                    else {                              // have an empty place
                        shmptr[first_empty] = process_id;
                        strcpy(msg.m_text, "1 | The process ADDED!");
                    }}
                break;
            case 2:             // -- cheking if a process registered -- //
                flag = false;
                is_exist = exist_pid(process_id);
                if (is_exist == -1)         // process is found, its exist
                    strcpy(msg.m_text, "0 | NOT EXIST");
                else                        // process not found
                    strcpy(msg.m_text, "1 | PROCESS EXISTS");
                break;
            case 3:             // -- removing a process -- //
                is_exist = exist_pid(process_id);
                if (is_exist != -1)         // process is found
                    shmptr[is_exist] = 0;
                flag = true;
                break;
        }
        shmptr[0] = 0; // this server done to write to memory
        msg.mtype = process_id;
        if (!flag)
            if (msgsnd(msqid, &msg, MAX_LEN, 0) == -1) {
                perror("msgsnd failed");
                exit(EXIT_FAILURE);
            }
    }
    return EXIT_SUCCESS;
}
// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Signal handler for SIGINT
// close the queue and the shared memory
void catch_intr(int s) {
    if (s == SIGINT)
        if (msgctl(msqid, IPC_RMID, NULL) == -1) {
            perror("msgctl failed");
            exit(EXIT_FAILURE);
        }if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("shmctl failed");
        exit(EXIT_FAILURE);
    }exit(EXIT_SUCCESS);
}
//---------------------------------------------------------------------------//
// This function get a process id and a array of process' id
// Searching if the desired process exist in the array.
// return first place is empty, otherwise - returns -1
int search_pid(pid_t wanted) {
    int i;                      // index for running on array
    int empty = ARR_SIZE;      // in the worst case - there is no empty cells
    bool found_empty = false;       // check if we found an empty place already
    for (i = 1; i < ARR_SIZE; i++) {
        if (shmptr[i] == wanted)
            return -1;
        if (!found_empty && shmptr[i] == 0) {   // found an empty cell
            empty = i;
            found_empty = true;
        }}
    return empty;       // return first empty
}
//---------------------------------------------------------------------------//
// This function get a process id and a array of process' id
// Searching if the desired process exist in the array.
// return his index if found, otherwise - returns (-1)
int exist_pid(pid_t wanted) {
    int i;           // index for running on array
    for (i = 1; i < ARR_SIZE; i++)
        if (shmptr[i] == wanted)
            return i;
    return -1;
}
//---------------------------------------------------------------------------//
// This function get a key, a message queue id and a char
// Checks if can open the queue using the char as a 'type'
void open_queue(key_t *_key, int *_msqid, char _type) {
    if (((*_key) = ftok("/.", _type)) == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }if (((*_msqid) = msgget(*_key, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
}
//---------------------------------------------------------------------------//
// This function get a key, a shared memory id and a char
// Checks if can open the queue using the char as a 'type'
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
