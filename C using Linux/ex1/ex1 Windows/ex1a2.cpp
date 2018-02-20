#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <windows.h> 
#include <time.h>

// ============================== Defines ================================== //
#define SIZE 100			// size of array of numbers

// ============================= Prototypes ================================ //
void bubble_sort(int arr[]);
void swap(int *xp, int *yp);

// ================================ Main =================================== //
int main(int argc, char * argv[]) {
	int arr[SIZE];              // array of integers
	char num[10];
	char str[SIZE * 4];	        // casted array of strings

	char path[] = "ex1a5.exe";
	time_t start,			 // the time passed when the program started
		   end;              // the time passed when the program ended

	int i,					// index for running on numbers array
		j = 0;				// index for running on strings array

	str[0] = '\0';			// make 'str' null-terminated string

	// cast to strings-array
	for (i = 1; i < argc; i++) {
		arr[j] = atoi(argv[i]);
		j++;
	}

	start = time(NULL);
	bubble_sort(arr);
	end = time(NULL);

	printf("Bubble Sort: %d %d %d\n", (int)end - (int)start, arr[0],
		arr[SIZE - 1]);
	strcat(str, path);

	for (i = 0; i < argc - 1; i++) {
		sprintf(num, "%d ", arr[i]);
		strcat(str, num);
	}

	//--- start of CreateProcces
	//Gives info on the thread and process for the new process
	PROCESS_INFORMATION pif;
	STARTUPINFOA si;				// Defines how to start the program

	ZeroMemory(&si, sizeof(si));    // Zero the STARTUPINFO struct
	si.cb = sizeof(si);             // Must set size of structure
	BOOL bRet = CreateProcessA(path,
		str, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pif);

	if (!bRet)
		puts("create process failed");
	else {
		WaitForSingleObject(pif.hProcess, INFINITE);
		CloseHandle(pif.hProcess);
		CloseHandle(pif.hThread);
	}
	//--- end of CreateProcces
}

// ------------------------------------------------------------------------- //
// This function get an integers array
// Performs a Bubble-sort search on the array
void bubble_sort(int arr[]) {
	int i, j;       // indexes for running on numbers array

	for (i = 0; i < SIZE - 1; i++)

		// Last i elements are already in place
		for (j = 0; j < SIZE - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}
// --------------------------------------------------------------------------//
// Function get 2 pointers of integers.
// Swap the values between these 2 numbers.
void swap(int *xp, int *yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}