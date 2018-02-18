/******************************************************************************
* Exr. #4b3: This Program us as CLIENT process.
*            the client connects to both servers, if it can't registered.
*            if it sucseeded:
*            reads a char from user, if it 'n' referring to operation on
*            number, if it 's' referring to operation on string.
*            (to the application server)
*            when reads 'e' - the client process ends and calls the register
*            server to be removed.
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
* Output: 0/1 by the answer the (application) server
*         Informative messages for the user
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

// ============================== Defines ================================== //
#define FIFO_NAME1 "reg_server_read"
#define FIFO_NAME2 "reg_server_write"
#define FIFO_NAME3 "app_server_read"
#define FIFO_NAME4 "app_server_write"
#define STR_LEN    100	           // the size of the message, in chars

// ============================= Prototypes ================================ //
void open_files();

// ============================== Globals ================================== //
FILE *reg_w,            // file descriptor of registry server for write
     *reg_r,            // file descriptor of registry server for read
     *app_w,            // file descriptor of application server for write
     *app_r;            // file descriptor of application server for read

// ================================ Main =================================== //
int main()
{
	char s[] = "         ";
	int num;                   // keeping an integer
    char ans;              // keep user answer of the operation he willing to
	char txt[STR_LEN];          // keep message info

    open_files();               // open the (necessary) files

    /// register to server 1 (registry) ///
	sprintf(s, "%d", (int) (getpid()));
	strcpy(txt, "1");           // 1 = code to register new process
	strcat(txt, s);
	fprintf(reg_w, "%s\n", txt);
	fflush(reg_w);

    /// now we can try applications from the applications server ///
	while (1) {
		puts("Enter a char please: ( s - string / n - number / e - exit)");
		scanf(" %c", &ans);

		switch (ans) {
		case 'n':           // working on a number
			puts("Enter a number please:");
			scanf("%d", &num);
			sprintf(s, "%d", num);
			strcpy(txt, "1");           // 1 = code for handle numbers
			strcat(txt, s);
			break;

		case 's':           // working on string
			puts("Enter a string please:");
			scanf("%s", s);
			strcpy(txt, "2");           // 1 = code for handle strings
			strcat(txt, s);
			break;

		case 'e':           //- ask to exit -
			strcpy(txt, "3");           // 3 = code for handle delete process
			sprintf(s, "%d", (int) getpid());
			strcat(txt, s);
			fprintf(reg_w, "%s\n", txt);
			fflush(reg_w);

            // close all files
			fclose(reg_r);
			fclose(reg_w);
			fclose(app_r);
			fclose(app_w);
			exit(0);

		default:        // unknown request
			continue;
		}
        // returns answer from named pipe
		fprintf(app_w, " %s\n", txt);
		fflush(app_w);
		fscanf(app_r, " %s", txt);
		printf("%d\n", txt[0] - 48);
	}

	return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
//---------------------------------------------------------------------------//
// This function open all the necessary files in the program
void open_files()
{
    if (!(reg_w = fopen(FIFO_NAME1 , "w"))) {
        perror("cannot open fifo file for w");
        exit(EXIT_FAILURE);
    }
    if (!(reg_r = fopen(FIFO_NAME2 , "r"))) {
        perror("cannot open fifo file for r");
        exit(EXIT_FAILURE);
    }
    if (!(app_w = fopen(FIFO_NAME3 , "w"))) {
        perror("cannot open fifo file for w");
        exit(EXIT_FAILURE);
    }
    if (!(app_r = fopen(FIFO_NAME4 , "r"))) {
        perror("cannot open fifo file for r");
        exit(EXIT_FAILURE);
    }
}