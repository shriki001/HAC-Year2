/******************************************************************************
* Exr. #1b4: This program mimic the shell environment.
*			 This program through the arguments vector a name of a file that it
*			 will write on him the commands the of user's input.
*			 The program runs as a loop, each iteration the user can input the
*			 following commands:
*			 history, echo, ps and exit.
*			 the program perfroms the requseted commnad (if exist).
*			 when the command exit requested - the program wiil delete the
*			 history file and finally ends.
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: An input file name (at first as argument), than a series of commands.
*
* -----------------------------------------------------------------------------
* Output: Accordingly to each command output.
*
* =============================================================================
******************************************************************************/

// ============================== Include ================================== //
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <stdbool.h>

// ============================== Defines ================================== //
#define SIZE 100            // the maximum size of a row input

// ============================= Prototypes ================================ //
void can_fork(pid_t process);
bool is_known_command(char* _command);
void can_open(FILE* a_file);
void history(FILE * inp_fd, char * out_file_name);
void echo(char * input_line);
void ps(char input_line[]);

// ================================ Main =================================== //
int main(int argc, char** argv) {
    char str[SIZE];					// keep a row of the user's input
    char *ptr;
	pid_t status;                   // The current process' ID
    FILE* inp_fd;					// File descriptor for input file

	inp_fd = fopen(argv[1], "w");
    can_open(inp_fd);

// The main loop of the program, works as a "mini-shell"
// As long as the user request
    do {
        // try to read the next row of the user's input
        fgets(str, SIZE, stdin);
        fprintf(inp_fd,"%s",str);
        ptr = strtok(str, " ");

        if (ptr[strlen(ptr)-1]=='\n')
            ptr[strlen(ptr)-1]='\0';

        if (is_known_command(ptr)){
            status = fork();
            can_fork(status);
            // can fork a child for a command
            if (status == 0) {
                char * _command = ptr;

                if ((strcmp(_command , "history") == 0))
                    history(inp_fd,argv[1]);
                else if ((strcmp(_command , "echo") == 0))
                    echo(&str[5]);
                else if ((strcmp(_command , "ps") == 0))
                    ps(strtok(NULL, " "));

               exit(EXIT_SUCCESS);
            }
            waitpid(status, NULL, 0);
        }

        else if (strcmp(str, "exit") == 0)
            break;
        // the command the user entered doesn't known
        else
            printf("%s: bad command\n", ptr);


    } while (1);

    // closing and removing the history file ,as the loop ended.
    fclose(inp_fd);
    sprintf(str,"rm %s",argv[1]);
    system(str);

	return EXIT_SUCCESS;
}

// =============================== Functions =============================== //
//---------------------------------------------------------------------------//
// This function get a string tha present a command to execute.
// Return true if the command is known, otherwise - return false.
bool is_known_command(char* _command)
{
    return ((strcmp(_command, "history") == 0)) ||
           ((strcmp(_command, "echo") == 0)) ||
           ((strcmp(_command, "ps") == 0)) ;

}

//---------------------------------------------------------------------------//
// This function gets a file descriptor (of input file) and a string.
// The program shows on screen the content of this file (all the commands the
// user entered).
void history(FILE * inp_fd, char * out_file_name)
{
    fclose(inp_fd);

    char *args[] = { "./history", out_file_name, NULL };
    if (execvp("./history", args) != 0) {
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }
    inp_fd = fopen(out_file_name, "a");
    can_open(inp_fd);
}

//---------------------------------------------------------------------------//
// This function gets a string.
// The function shows on screen this string's content.
void echo(char * input_line)
{
    if (strlen(input_line)==0)
        input_line="\n";

    char *args[] = {"./echo" , input_line , NULL};
    if (execvp("./echo" , args) != 0) {
        perror("execvp() failed");
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function gets a string.
// The program shows on screen father's current process ID and/or current
// process ID
void ps(char input_line[])
{
    // case without arguments
    if (input_line == NULL) {
        if (execl("./ps" ,"./ps", NULL) != 0) {
            perror("execl() failed");
            exit(EXIT_FAILURE);
        }
    }

    // case with arguments
    else {
        if (execl("./ps" , "./ps" , input_line, NULL) != 0) {
            perror("execl() failed");
            exit(EXIT_FAILURE);
        }
    }
}
//---------------------------------------------------------------------------//
// THis function gets a file descriptor
// checks if file can be opened successfully
void can_open(FILE* a_file)
{
    if (a_file == NULL) {
        fputs("Cannot open file \n" , stderr);
        exit(EXIT_FAILURE);
    }
}

//---------------------------------------------------------------------------//
// This function gets a procees id.
// checks if the progrem can execute fork()
void can_fork(pid_t process)
{
    if (process < 0) {
        perror("Cannot fork!\n");
        exit(EXIT_FAILURE);
    }
}
