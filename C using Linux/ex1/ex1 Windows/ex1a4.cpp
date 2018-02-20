// ============================== Include ================================== //
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// ============================== Defines ================================== //
#define SIZE 100		   // the range of number to be raffled

// ============================= Prototypes ================================ //
void search(int arr[], int num);

// ================================ Main =================================== //
int main(int argc, char ** argv) {

	int arr[SIZE];			  // array of integers
	time_t start,			 // the time passed when the program started
		   end;              // the time passed when the program ended
	int i,					// index for running on numbers array
		j = 0;				// index for running on strings array

// cast to strings-array
	for (i = 1; i < argc; i++) {
		arr[j] = atoi(argv[i]);
		j++;
	}

	srand((unsigned)time(NULL));
	int num,av=0;
	for (i = 0; i < SIZE; i++) {
		num = rand() % (SIZE);
		start = time(NULL);
		search(arr, num);
		end = time(NULL);
		av += (int)end - (int)start;
	}

	printf("Ordinary Search: %d \n", av / SIZE);
	return EXIT_SUCCESS;
}

// ------------------------------------------------------------------------- //
// This function get an integers array and an integer number.
// Performs a sequential search on the array.
void search(int arr[], int num) {
	int i;
	for (i = 0; i < SIZE; i++)
		if (arr[i] == num)
			return;
}