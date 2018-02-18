/******************************************************************************
* Exr. #1a: The program defines a (non-dynamic) array by size of 100,000
*			cells and magrila into it values in range [0,index].
*			The program creates 2 process that will sort the array (in 2
*			separate ways). each child(-process) prints the time it took
*			to him to finish, and more information about him.
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* login	   		: kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: Nothing.
*
* -----------------------------------------------------------------------------
* Output: from The 2 first childern:
*         time it took to do it sort, which sort it performed, the minimum
*         and the maximum values in the array.
*
*         annoncment the the 3rd child(son of the father process) performed
*         a sequential search and time it took, as well the same data for the
*         4th child (son of the 1st child, peformed a binary search).
*         
* =============================================================================
******************************************************************************/

// ============================== Include ================================== //
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <wait.h>

// ============================== Defines ================================== //
#define SIZE 100000            // the size of the numbers array
#define LEN 100                // the range of number to br mugral

// ============================= Prototypes ================================ //
void bubble_sort(int arr[]);
void quicksort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void swap(int *num1, int *num2);
void sequential_search(int arr[], int num);
void binary_search(int arr[], int low, int size, int num);
void do_child(int i, int arr[]);
void child_a(int arr[]);
void child_b(int arr[]);
void child_c(int arr[]);
void child_d(int arr[]);
void can_fork(pid_t process);

// ================================ Main =================================== //
int main() {
	pid_t status;			// keep the current process ID
	int arr[SIZE];          // array to keep all the numbers
    int i;					// index for counting children
	srand((unsigned)time(NULL));


	for (i = 0; i < SIZE; i++)
		arr[i] = rand() % (SIZE);

	// the father process produce 3 children 
	for (i = 0; i < 3; i++) {
		status = fork();
		can_fork(status);
		if (!status) {
			do_child(i, arr);
			exit(EXIT_SUCCESS);
		}
		waitpid(status, NULL, 0);		// wait for the lase child process
	}

	return EXIT_SUCCESS;
}

// =============================== Functions =============================== //
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array and an index.
// peforms the right function to the n-th child
void do_child(int i, int arr[]) {
	switch (i) {
        case 0:
            child_a(arr);
            break;
        case 1:
            child_b(arr);
            break;
        case 2:
            child_c(arr);
            break;
    }
}

//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which sorts the array using the
// quick-sort algorithm.
void child_a(int arr[]) {
	pid_t status;			// keep the current process ID
    time_t start,              // the time passed when the program started
           end;                // the time passed when the program ended

    start = time(NULL);
	bubble_sort(arr);
	end = time(NULL);
	printf("Bubble Sort: %d %d %d\n", (int) end - (int) start, arr[0],
			arr[SIZE - 1]);
	status = fork();
	can_fork(status);
	if (!status) {
		child_d(arr);
		exit(EXIT_SUCCESS);
	}
}

//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which sorts the array using the
// quick-sort algorithm.
void child_b(int arr[]) {
    time_t start,              // the time passed when the program started
           end;                // the time passed when the program ended

    start = time(NULL);
	quicksort(arr, 0, SIZE - 1);
	end = time(NULL);
	printf("Quick Sort: %d %d %d\n", (int) end - (int) start, arr[0],
			arr[SIZE - 1]);

}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which magril a number 100 times and
// search if he exists in the array using seqenetial search.
void child_c(int arr[]) {
	int i,                  // index for running on the array's cells
        num,                // keep the number that hugral
        total_time = 0;        // the total time the searchs took (in seconds)
	time_t start,              // the time passed when the program started
           end;                // the time passed when the program ended

	for (i = 0; i < LEN; i++) {
		num = rand() % (SIZE);
		start = time(NULL);
		sequential_search(arr, num);
		end = time(NULL);
		total_time += (int) end - (int) start;
	}
	printf("Serial Search: %d \n", total_time / LEN);

}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which magril a number 100 times and
// search if he exists in the array using binary search.
void child_d(int arr[]) {
    int i,                  // index for running on the array's cells
        num,                // keep the number that hugral
        total_time = 0;        // the total time the searchs took (in seconds)
    time_t start,              // the time passed when the program started
           end;                // the time passed when the program ended

	for (i = 0; i < LEN; i++) {
		num = rand() % (SIZE);
		start = time(NULL);
		binary_search(arr, 0, SIZE - 1, num);
		end = time(NULL);
		total_time += (int) end - (int) start;
	}
	printf("Binary Search: %d \n", total_time / LEN);
}

//---------------------------------------------------------------------------//
// This function get an integers array and an integer number.
// Performs a sequential search on the array.
void sequential_search(int arr[], int num) {

    int i;                  // index for running on the array's cells

    for (i = 0; i < SIZE; i++)
		if (arr[i] == num)
			break;
	return;
}
//---------------------------------------------------------------------------//
// This function get an integers array
// Performs a Bubble-sort search on the array
void bubble_sort(int arr[]) {
	int i, j;                  // indexes for running on the array's cells

	for (i = 0; i < SIZE - 1; i++)
		// Last i elements are already in place
		for (j = 0; j < SIZE - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);

}

//---------------------------------------------------------------------------//
void swap(int *num1, int *num2) {

	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

//---------------------------------------------------------------------------//
int partition(int arr[], int low, int high) {

	int pivot = arr[high];    // pivot
	int i = (low - 1), j;  // Index of smaller element

	for (j = low; j <= high - 1; j++) {
		// If current element is smaller than or
		// equal to pivot
		if (arr[j] <= pivot) {
			i++;    // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

//---------------------------------------------------------------------------//
void quicksort(int array[], int low, int high) {
	if (low < high) {
		/* pi is partitioning index, array[p] is now at right place */
		int pi = partition(array, low, high);

		// Separately sort elements before partition and after partition
		quicksort(array, low, pi - 1);
		quicksort(array, pi + 1, high);
	}
}

//---------------------------------------------------------------------------//
// This function get an integers array and an integer number.
// Performs a binary search on the array
void binary_search(int arr[], int low, int size, int num) {
	while (low <= size) {
		int mid = low + (size - low) / 2;

		// Check if x is present at mid
		if (arr[mid] == num)
			break;
		
		if (arr[mid] < num)  // If x greater, ignore left half
			low = mid + 1;
			
		else  // If x is smaller, ignore right half
			size = mid - 1;
	}
}

//---------------------------------------------------------------------------//
// This function gets a procees id.
// checks if the progrem can execute fork()
void can_fork(pid_t process) {
    if (process < 0) {
        perror("Cannot fork!\n");
        exit(EXIT_FAILURE);
    }
}
