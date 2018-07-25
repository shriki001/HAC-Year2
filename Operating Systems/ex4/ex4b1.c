/******************************************************************************
* Exr. #4b1: This Program used as the REGISTRY SERVER.
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
* login	   	    : kfirma / michaelshr
* Course/Year   : OS1 2018
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
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>

// ============================== Defines ================================== //
#define ARR_SIZE   10
#define STR_LEN    100	               // the size of the pid array
#define FIFO_NAME1 "reg_server_read"
#define FIFO_NAME2 "reg_server_write"

// ============================= Prototypes ================================ //
void catch_intr(int s);
int search_pid(pid_t wanted, pid_t arr[]);
int exist_pid(pid_t wanted, pid_t arr[]);
void open_files();

// ============================== Globals ================================== //
FILE *fdw,        // file descriptor for write
     *fdr;        // file descriptor for read

// ================================ Main =================================== //
int main()
{
    bool flag = false;
    char *s;
    int process_id;                  // keep process's id
    int first_empty;                // indicate first cell is empty
    int is_exist;                   // indicate if process exist

    long type;                      // keep message type
    char txt[STR_LEN];          // keep message info

    // make 2 fifo-s
	if (mkfifo(FIFO_NAME1, S_IFIFO | 0600) == -1 &&
	errno != EEXIST) {
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}
	if (mkfifo(FIFO_NAME2, S_IFIFO | 0600) == -1 &&
	errno != EEXIST) {
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}

    open_files();               // open the (necessary) files

	signal(SIGINT, catch_intr);

	// Creating an array to keep the process' id which registered on the
	// server - initialized to zeros (a process id != 0)
	pid_t pid_array[ARR_SIZE] = { 0 };

	// the main loop of the running server
	while (1) {
		while (fscanf(fdr, " %s", txt) != EOF) {
			s = &txt[1];
			process_id = atoi(s);
			s = NULL;
			type = (long) txt[0] - 48;
			switch (type) {
			case 1:
				// -- adding a process -- //
				flag = false;
				first_empty = search_pid(process_id, pid_array);
				if (first_empty == -1)              // process is found
					strcpy(txt, "0 | CANT ADD (process is exist!)");
				else {        // process is not found
					if (first_empty == ARR_SIZE)     // dont have an empty place
						strcpy(txt, "2 | CANT ADD (array is FULL!)");
					else {                              // have an empty place
						pid_array[first_empty] = process_id;
						strcpy(txt, "1 | The process ADDED!");
					}
				}
				break;
			case 2:             // -- cheking if a process registered -- //
				flag = false;
				is_exist = exist_pid(process_id, pid_array);
				if (is_exist == -1)         // process is found, its exist
					strcpy(txt, "0 | NOT EXIST");
				else
					// process not found
					strcpy(txt, "1 | PROCESS EXISTS");

				break;

			case 3:             // -- removing a process -- //
				is_exist = exist_pid(process_id, pid_array);
				if (is_exist != -1)         // process is found
						{
					pid_array[is_exist] = 0;
				}
				flag = true;
				break;
			}

			if (!flag) {
				fprintf(fdw, " %s\n", txt);
				fflush(fdw);
			}
		}
	}
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Singal handler for SIGINT
// close the all files and removes them
void catch_intr(int s)
{
	char str[STR_LEN];
	if (s == SIGINT) {
		fclose(fdr);
		fclose(fdw);
		sprintf(str, "rm %s", FIFO_NAME1);
		system(str);
		sprintf(str, "rm %s", FIFO_NAME2);
		system(str);
	}
	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------//
// This function get a process id and a array of process' id
// Searching if the desired process exist in the array.
// return first place is empty, otherwise - returns -1
int search_pid(pid_t wanted, pid_t arr[]) {
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
int exist_pid(pid_t wanted, pid_t arr[]) {
	int i;           // index for running on array

	for (i = 0; i < ARR_SIZE; i++)
		if (arr[i] == wanted)
			return i;

	return -1;
}

//---------------------------------------------------------------------------//
// This function open all the necessary files in the program
void open_files()
{
    if (!(fdr = fopen(FIFO_NAME1, "r"))) {
        perror("cannot open fifo file for r");
        exit(EXIT_FAILURE);
    }
    if (!(fdw = fopen(FIFO_NAME2, "w"))) {
        perror("cannot open fifo file for w");
        exit(EXIT_FAILURE);
    }
}
