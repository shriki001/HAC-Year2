/******************************************************************************
* Exr. #4b2: This Program used as the APPLICATIONS SERVER.
*            keeps the process id-s that connected to the server, this server
*            create a queue and allows 2 operations:
*            1) check a primery of a number
*            2) check if a string is palindrome
*
*            If the process exist on th database, if the result is "true"
*            returns 1, if "false" - returns 0. to add, returns 0.
*
*            If the process isn't exist on th database, returns -1
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* login	   		: kfirma / michaelshr
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
#define FIFO_NAME1 "app_server_read"
#define FIFO_NAME2 "app_server_write"
#define FIFO_NAME3 "reg_server_read"
#define FIFO_NAME4 "reg_server_write"
#define STR_LEN    100	           // the size of the message, in chars

// ============================= Prototypes ================================ //
void catch_intr(int s);
bool is_prime(int n);
bool is_palidrom(char s[]);
void open_files();

// ============================== Globals ================================== //
FILE *client_w,            // file descriptor of client for write
     *client_r,            // file descriptor of client for read
     *serv1_r,            // file descriptor of registry server for write
     *serv1_w;            // file descriptor of registry server for read

// ================================ Main =================================== //
int main() 
{
	char txt[STR_LEN];          // keep message info
    int ans;     // keep user answer of the operation he willing to

    if (mkfifo(FIFO_NAME1, S_IFIFO | 0600) == -1 && errno != EEXIST) {
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}

	if (mkfifo(FIFO_NAME2, S_IFIFO | 0600) == -1 && errno != EEXIST) {
		perror("cannot create fifo file");
		exit(EXIT_FAILURE);
	}

    open_files();               // open the (necessary) files

	signal(SIGINT, catch_intr);         // signal handler

	char s[] = "             ";
	char *c;

	while(1) {
		while (fscanf(client_r , "%s" , txt) != EOF) {
			c = &txt[1];
			switch ((int) txt[0] - 48) {
				case 1:         // case asked a number
					ans = is_prime(atoi(c));
					break;

				case 2:         // case asked a string
					ans = is_palidrom(c);
					break;

				default:        // unknown case
					continue;
			}

			sprintf(s , "%d" , ans);
			fprintf(client_w , " %s\n" , s);
			fflush(client_w);
		}
	}
	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// Signal handler for SIGINT
// close the all files and removes them
void catch_intr(int s)
{
	char str[STR_LEN];

    if (s == SIGINT) {
		fclose(client_r);
		fclose(client_w);
		fclose(serv1_r);
		fclose(serv1_w);
		sprintf(str, "rm %s", FIFO_NAME1);
		system(str);
		sprintf(str, "rm %s", FIFO_NAME2);
		system(str);
	}

	exit(EXIT_SUCCESS);
}

//---------------------------------------------------------------------------//
// This function get an integer number
// Return true if it prime number. otherwise - return false.
bool is_prime(int n)
{
	int div;         // index for runnig on array
	for (div = 2; div <= n / 2; div++)
		if (n % div == 0)
			return false;
	return true;
}

//---------------------------------------------------------------------------//
// This function get a string
// Return true if it a palindrome. otherwise - return false.
bool is_palidrom(char s[])
{
	int i;          // index for runnig on array
	size_t size = strlen(s);   // size of the string
	for (i = 0; i < size / 2; i++)
		if (s[i] != s[size - 1 - i])
			return false;
	return true;
}
//---------------------------------------------------------------------------//
// This function open all the necessary files in the program
void open_files()
{
    if (!(client_r = fopen(FIFO_NAME1, "r"))) {
		perror("cannot open fifo file for r");
		exit(EXIT_FAILURE);
	}

	if (!(client_w = fopen(FIFO_NAME2, "w"))) {
		perror("cannot open fifo file for w");
		exit(EXIT_FAILURE);
	}

	if (!(serv1_r = fopen(FIFO_NAME3, "r"))) {
		perror("cannot open fifo file for r");
		exit(EXIT_FAILURE);
	}
	if (!(serv1_w = fopen(FIFO_NAME4, "w"))) {
		perror("cannot open fifo file for w");
		exit(EXIT_FAILURE);
	}
}
