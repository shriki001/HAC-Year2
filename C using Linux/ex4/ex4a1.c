/******************************************************************************
* Exr. #4a1: This Program used as the REGISTRY SERVER.
*            keeps the process id-s that connected to the server, this server
*            create a queue and allows 3 operations:
*            1) add a new process
*            2) check if a process if registered on system
*            3) removal of existing process
*
*            when get code:
*            1: if successed to add, return 0.
*               if not - return 1 because it alredy exist, and return 2
*               because it not exist at all.
*            2: if exist -return 1, else -return 0.
*            3: just remove from the database
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* login	   	: kfirma / michaelshr
* Course/Year  : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: Nothing.
*
* -----------------------------------------------------------------------------
* Output: Nothing.
*
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
int search_pid(pid_t wanted, pid_t arr[]);
int exist_pid(pid_t wanted, pid_t arr[]);

void open_queue(key_t* _key, int* _msqid, char _type);

// ============================== Globals ================================== //
int msqid;

// ================================ Main =================================== //
int main()
{
	struct msgbuf1 msg;
	bool flag = false;      // flag for checking if we still run
	key_t key;             // key of message queue of this server (registry)

    int process_id;                  // keep process's id
    int first_empty;                // indicate first cell is empty
    int is_exist;                   // indicate if process exist

    // Creating an array to keep the process' id which registered on the
    // server - initialized to zeros (a process id != 0)
	pid_t pid_array[ARR_SIZE] = { 0 };

	signal(SIGINT, catch_intr);     //  signal handler

    open_queue(&key, &msqid, 'a');      // try to open the message queue

    // the main loop of the running server
	while (1) {
		if (msgrcv(msqid, &msg, MAX_LEN, 0, 0) == -1) {
			perror("msgrcv failed");
			exit(EXIT_FAILURE);
		}

        process_id = atoi(msg.m_text);

        switch (msg.mtype) {
		case 1:              // -- adding a process -- //
			flag = false;

			first_empty = search_pid(process_id, pid_array);
			if (first_empty == -1)              // process is found
				strcpy(msg.m_text, "0 | CANT ADD (process is exist!)");
			else        // process is not found
            {
                if (first_empty == ARR_SIZE)        // dont have an empty place
                    strcpy(msg.m_text, "2 | CANT ADD (array is FULL!)");
                else {                              // have an empty place
                    pid_array[first_empty] = process_id;
                    strcpy(msg.m_text, "1 | The process ADDED!");
                }
            }
			break;

		case 2:             // -- cheking if a process registered -- //
			flag = false;

			is_exist = exist_pid(process_id, pid_array);
			if (is_exist == -1)         // process is found, its exist
				strcpy(msg.m_text, "0 | NOT EXIST");
			else                        // process not found
				strcpy(msg.m_text, "1 | PROCESS EXISTS");

			break;

		case 3:             // -- removing a process -- //
			is_exist = exist_pid(process_id, pid_array);
			if (is_exist != -1)         // process is found
				pid_array[is_exist] = 0;
			flag = true;
			break;
		}

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
// close the queue
void catch_intr(int s)
{
	if (s == SIGINT)
		if (msgctl(msqid, IPC_RMID, NULL) == -1) {
			perror("msgctl failed");
			exit(EXIT_FAILURE);
		}
	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------//
// This function get a process id and a array of process' id
// Searching if the desired process exist in the array.
// return first place is empty, otherwise - returns -1
int search_pid(pid_t wanted, pid_t arr[])
{
    int i;                      // index for running on array
	int empty = ARR_SIZE;      // in the worst case - there is no empty cells
	bool found_empty = false;       // check if we found an empty place already

    for (i = 0; i < ARR_SIZE; i++) {
		if (arr[i] == wanted)
			return -1;
		if (!found_empty && arr[i] == 0) {   // found an empty cell
			empty = i;
			found_empty = true;
		}
	}

	return empty;       // return first empty
}

//---------------------------------------------------------------------------//
// This function get a process id and a array of process' id
// Searching if the desired process exist in the array.
// return his index if found, otherwise - returns (-1)
int exist_pid(pid_t wanted, pid_t arr[])
{
	int i;           // index for running on array

	for (i = 0; i < ARR_SIZE; i++)
		if (arr[i] == wanted)
			return i;

	return -1;
}

//---------------------------------------------------------------------------//
// This function get a key, a message queue id and a char
// Checks if can open the queue using the char as a 'type'
void open_queue(key_t* _key, int* _msqid, char _type)
{
    if (((*_key) = ftok("/.", _type)) == -1) {
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    if (( (*_msqid) = msgget(*_key, 0600 | IPC_CREAT | IPC_EXCL)) == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }
}
