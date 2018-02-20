/******************************************************************************
* Thread for Linux
* Exr. #1b3: This program mimic the shell's command "ps". 
*			 If the program executed without any argument, shows the process ID 
*			 of the current process's father over the screen. 
*			 In case it executed with the argument "-a", shows the process IDs
*			 of the current process and it's father over the screen.
*			 If executed on any other way - an error message will be prompted
*			 and program will be teriminated.
*			  
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input:  No, except an argumant.
*
* -----------------------------------------------------------------------------
* Output: The process ID of the current process's father (and the current 
*		  process ID if "-a" was entered as argument).  
*         
******************************************************************************/
// ============================== Include ================================== //
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// ================================ Main ================================== //
int main(int argc, char** argv)
{
    if (argc == 1) 		// no arguments
        printf("%d\n", getppid());
    
	else if (argc == 2 && (strcmp(argv[1] ,"-a\n") == 0))
        printf("%d\n%d\n", getpid(), getppid());

    else{			// bad argument
        fprintf(stderr,"%s Wrong or missing argument \n", argv[0]);
        exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
