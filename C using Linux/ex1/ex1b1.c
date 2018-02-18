/******************************************************************************
* Exr. #1b1: This program mimic the shell's command "history". 
*			 This program through the arguments vector a name of a file.
*			 Opens this file for reading, and reading it row by row.
*		     The program shows the file's content over the screen (output to
*			 stdout file).
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: A string represents a name of a file (through vector argument).
*
* -----------------------------------------------------------------------------
* Output: The content of the file's named on the string.
*         
*         
******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>

// ============================== Defines ================================== //
#define ROW_SIZE 100			// the size of the input (row user entered)

// ================================ Main ================================== //
int main(int argc, char** argv) {
    FILE * inp_fd;
    char input_line[ROW_SIZE];

    if (argc != 2){
        fprintf(stderr,"%s Usage: <input file> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    inp_fd = fopen(argv[1], "a+");

    // checking if file opened successfully
    if (inp_fd == NULL) {
        fputs("Cannot open file \n", stderr);
        exit(EXIT_FAILURE);
    }

	// reading and printing the file's content to stdout    
	fgets(input_line,ROW_SIZE, inp_fd);
    while (!feof(inp_fd))
    {
        printf("%s",input_line);
        fgets(input_line,ROW_SIZE, inp_fd);
    }
    fclose(inp_fd);
    return EXIT_SUCCESS;
}

