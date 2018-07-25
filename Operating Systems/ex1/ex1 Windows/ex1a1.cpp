#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
/******************************************************************************
* Thread for Windows
* Exr. #1b: The program defines a (non-dynamic) array by size of 100,000
*			cells and raffled into it values in range [0, index].
*			The program creates 2 process that will sort the array (in 2
*			separate ways). each child(-process) prints the time it took
*			to him to finish, and more information about him.
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
* Output: from The 2 first children:
*         time it took to do it sort, which sort it performed, the minimum
*         and the maximum values in the array.
*
*         announcement the the 3rd child(son of the father process) performed
*         a sequential search and time it took, as well the same data for the
*         4th child (son of the 1st child, performed a binary search).
*
* =============================================================================
******************************************************************************/

// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h> 
#include <time.h>

// ============================= Prototypes ================================ //
#define SIZE 100			// size of the numbers-array

// ================================ Main =================================== //
int main() {
	char num[10];
	char str[SIZE * 4];
	char args[SIZE * 4];
	args[0] = '\0';
	str[0] = '\0';
	char search_args[] = "ex1a4.exe";
	char quick_sort_args[] = "ex1a3.exe";
	char bubble_sort_args[] = "ex1a2.exe";

	char * a[3];			// vector of strings for arguments
	a[0] = bubble_sort_args;
	a[1] = quick_sort_args;
	a[2] = search_args;

	srand((unsigned)time(NULL));

	int i;				// index for running on array
	for (i = 0; i < SIZE; i++) {
		sprintf(num, "%d ", rand() % (SIZE));
		strcat(str, num);
	}

	// -------------------- CreateProcces -----------------------------------//

	PROCESS_INFORMATION pif[3];

	for (i = 0; i <3; i++)
	{
		args[0] = '\0';
		strcat(args, a[i]);
		strcat(args, str);

		STARTUPINFOA si;       //Defines how to start the program

		ZeroMemory(&si, sizeof(si));  //Zero the STARTUPINFO struct
		si.cb = sizeof(si);         //Must set size of structure
		BOOL _bRet = CreateProcessA(a[i],
			str, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pif[i]);

		if (!_bRet)
			puts("create process failed");	
	}

	//Wait for all processes

	for(i = 0; i < 3; i++) {
		WaitForSingleObject(pif[i].hProcess, INFINITE);
		CloseHandle(pif[i].hProcess);
		CloseHandle(pif[i].hThread);
	}
	getchar();
	// ----------------------------------------------------------------------//
	return EXIT_SUCCESS;
}