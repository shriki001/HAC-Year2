// ============================== Include ================================== //
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// ============================== Defines ================================== //
#define SIZE 100				// the range of number to be raffled

// ============================= Prototypes ================================ //
void binary_search(int arr[], int low, int size, int num);

// ================================ Main =================================== //
int main(int argc, char ** argv) {
	int arr[SIZE];
	time_t start,			 // the time passed when the program started
		   end;              // the time passed when the program ended
	int num,                // keep the number that raffled
		total_time = 0;        // the total time the search took (in seconds)
	int i,					// index for running on numbers array
		j = 0;				// index for running on strings array
	
	for (i = 1; i < argc; i++) {
		arr[j] = atoi(argv[i]);
		j++;
	}
	
	srand((unsigned)time(NULL));

	
	for (i = 0; i < 100; i++) {
		num = rand() % (SIZE);
		start = time(NULL);
		binary_search(arr, 0, SIZE - 1, num);
		end = time(NULL);
		total_time += (int)end - (int)start;
	}
	printf("Binary Search: %d \n", total_time / SIZE);
	
	return EXIT_SUCCESS;
}
// ------------------------------------------------------------------------- //
// This function get an integers array and an integer number.
// Performs a binary search on the array
void binary_search(int arr[], int low, int size, int num)
{
	while (low <= size)
	{
		int mid = low + (size - low) / 2;

		if (arr[mid] == num)        // Check if x is present at mid
			return;

		if (arr[mid] < num)      // If x greater, ignore left half
			low = mid + 1;

		else   // If x is smaller, ignore right half
			size = mid - 1;
	}
}