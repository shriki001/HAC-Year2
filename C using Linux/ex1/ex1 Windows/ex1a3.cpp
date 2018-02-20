// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ============================== Defines ================================== //
#define SIZE 100			// size of array of numbers

// ============================= Prototypes ================================ //
void quicksort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void swap(int *xp, int *yp);

// ================================ Main =================================== //
int main(int argc, char * argv[]) {
	time_t start,			// time that the program started
		   end;			    // time that the program ended
	int i,					// index for running on numbers array
		j = 0;				// index for running on strings array
	int arr[SIZE];			// array of integers
	
	// cast to strings-array
	for (i = 1; i < argc; i++) {
		arr[j] = atoi(argv[i]);
		j++;
	}
	
	start = time(NULL);
	quicksort(arr, 0, SIZE - 1);
	end = time(NULL);
	printf("Quick Sort: %d %d %d\n", 
		   (int)end - (int)start, arr[0], arr[SIZE - 1]);

	return EXIT_SUCCESS;
}

// =============================== Functions =============================== //
// --------------------------------------------------------------------------//
// Function get an array to be sorted, Starting index(low) and Ending index
// (high).
// Sort the array in the segment [low, high]
int partition(int arr[], int low, int high) {
	int pivot = arr[high];				  // pivot
	int i = (low - 1), j;				  // Index of smaller element

	for (j = low; j <= high - 1; j++) {
		// If current element is smaller than or equal to pivot
		if (arr[j] <= pivot) {
			i++;    
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

// --------------------------------------------------------------------------//
// The main function that implements Quick Sort
// get an integers array, 
void quicksort(int arr[], int low, int high) {
	if (low < high) {
		/* pi is partitioning index, array[p] is now at right place */
		int pi = partition(arr, low, high);

		// Separately sort elements before partition and after partition
		quicksort(arr, low, pi - 1);
		quicksort(arr, pi + 1, high);
	}
}

// --------------------------------------------------------------------------//
// This function get 2 pointers of integers.
// Swap the values between these 2 numbers.
void swap(int *xp, int *yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}