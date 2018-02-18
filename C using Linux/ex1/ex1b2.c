/******************************************************************************
* Exr. #1b2: This program mimic the shell's command "echo". 
*			 This program through the arguments vector a string.
*			 The program shows the strings's content over the screen (output to
*			 stdout file).
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: A string (through vector argument).
*
* -----------------------------------------------------------------------------
* Output: The string's content (on stdout).
*
* =============================================================================
******************************************************************************/
// ============================== Include ================================== //
#include <stdlib.h>
#include <stdio.h>

// ================================ Main ================================== //
int main(int argc, char** argv)
{
    if (argc != 2){
        fprintf(stderr,"%s Usage: <a string> \n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("%s", argv[1]);  // prints the string

    return EXIT_SUCCESS;
}
