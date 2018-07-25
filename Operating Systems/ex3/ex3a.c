/******************************************************************************
 * Exr. #3a: In this program the father process defines an array and pefroms the
 *           following 10 times:
 *           - raffles numbers into the array.
 *           - creates 2 children that sorting the array (first bubble-sort,
 *             the other one quick-sort)
 *
 *           The father process gets trough the pipe how much time it took
 *           for each one to sort the array.
 *
 *           the first raffles a number 10 times and creating a child process
 *           that search the array before it sorted (doing similarly a search
 *           after the array is sorted)
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
#include <unistd.h>
#include <time.h>
#include <wait.h>
// ============================== Defines ================================== //
#define SIZE 20000     // the size of the raffled numbers array
#define LEN 10         // the highest number that can be raffled
// ============================= Prototypes ================================ //
void bubble_sort(int arr[]);
void quicksort(int arr[], int low, int high);
int partition(int arr[], int low, int high);
void swap(int *num1, int *num2);
void search(int arr[], int num);
void binary_search(int arr[], int low, int size, int num);
void do_father(int pipe_descs[], int child, int c[]);
void do_bubble(int arr[], int pipe_descs[]);
void do_child(int i, int arr[], int pipe_descs[], int counter[]);
void child_a(int arr[], int pipe_descs[], int[]);
void child_b(int arr[], int pipe_descs[]);
void child_c(int arr[], int pipe_descs[]);
void child_d(int arr[], int pipe_descs2[]);
void can_fork(pid_t process);
void open_pipes(int _pipes[]);
// ================================ Main =================================== //
int main() {
	pid_t status;						// keep the current process ID
	int arr[SIZE];                     // array to keep all the numbers
	int i, j,p;				          // index for counting children
	int pipe_descs[2];                // the pipe descriptor
	int accums[4] = { 0 };            // keeping data of total tome of all
	char* str[4] = { "Bubble sort:", "Quick sort:", "Serial search",
			"Binary search:" };
	srand((unsigned) time(NULL));
	//-- running for 10 times the main loop --//
	for (j = 0; j < LEN; j++) {
		// insert raffled numbers into the array
		for (i = 0; i < SIZE; i++)
			arr[i] = rand() % (SIZE);
		open_pipes(pipe_descs);        // checking if we can open the pipe
		// the father process produce 2 children
		for (i = 0; i < 2; i++) {
			status = fork();
			can_fork(status);
			if (!status) {
				do_child(i, arr, pipe_descs, accums);
				exit(EXIT_SUCCESS);
			}
			waitpid(status, NULL, 0); // wait for the last child process
			do_father(pipe_descs, i, accums);
			open_pipes(pipe_descs); // open the pipes again after first close
		}
	}
	// showing final results (average times)
	for (p = 0; p < 4; p++)
		printf("%s \t%d \n", str[p], accums[p] / LEN);
	return EXIT_SUCCESS;
}
//---------------------------------------------------------------------------//
// Performs the father process role
void do_father(int pipe_descs[], int child, int c[]) {
	char buff[LEN];         //buffer to read input of children
	close(pipe_descs[1]);       // closing the pipe for writing
	dup2(pipe_descs[0], STDIN_FILENO);
	scanf("%s", buff);
	int x = atoi(buff);     // keeps the buff as number
	c[child] += x;          // adding the time to the totals array
	close(pipe_descs[0]);   // closing the pipe for reading
}
//---------------------------------------------------------------------------//
// Performs the bubble sort in array abd return the time it took in the pipe
void do_bubble(int arr[], int pipe_descs[]) {
	time_t start,              // the time passed when the program started
			end;                // the time passed when the program ended
	close(pipe_descs[0]);           // closing the pipe for reading
	dup2(pipe_descs[1], STDOUT_FILENO);
	start = time(NULL);
	bubble_sort(arr);
	end = time(NULL);
	printf("%d", (int) end - (int) start); // prints to pipe
	close(pipe_descs[1]);          // closing the pipe for writing
}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array, an index and the pipe.
// peforms the right function to the 1st/2nd child
void do_child(int i, int arr[], int pipe_descs[], int counter[]) {
	switch (i) {
	case 0:
		child_a(arr, pipe_descs, counter);
		break;
	case 1:
		child_b(arr, pipe_descs);
		break;
	}}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which find a number in array before sort
// and after sort, between he do bubble sort
void child_a(int arr[], int pipe_descs[], int c[]) {
	pid_t status;           // keeping the current process's Id
	int pipe_descs2[2];    // pipe descriptors
	open_pipes(pipe_descs2);
	status = fork();
	can_fork(status);           // check we can do fork
	if (!status) {      // a child doing serial search on unsorted array
		child_c(arr, pipe_descs2);
		exit(0);
	}
	waitpid(status, NULL, 0);
	do_father(pipe_descs2, 2, c);
	do_bubble(arr, pipe_descs);     // sorting the array (using bubble-sort)
	open_pipes(pipe_descs2);
	status = fork();
	can_fork(status);           // check we can do fork
	if (!status) {          // a child doing binary search on sorted array
		child_d(arr, pipe_descs2);
		exit(0);
	}	do_father(pipe_descs2, 3, c);
}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which sorts the array using the
// quick-sort algorithm.
void child_b(int arr[], int pipe_descs[]) {
	time_t start,              // the time passed when the program started
			end;                // the time passed when the program ended
	close(pipe_descs[0]);       // closing the pipe for reading
	dup2(pipe_descs[1], STDOUT_FILENO);
	start = time(NULL);
	quicksort(arr, 0, SIZE - 1);
	end = time(NULL);
	printf("%d", (int) end - (int) start);  // prints to pipe
	close(pipe_descs[1]);    // closing the pipe for writing
}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which raffle a number 100 times and
// search if he exists in the array using sequential search.
void child_c(int arr[], int pipe_descs[]) {
	int i,                  // index for running on the array's cells
			num,                // keep the number that hugral
			total_time = 0;      // the total time the searchs took (in seconds)
	time_t start,              // the time passed when the program started
			end;                // the time passed when the program ended
	close(pipe_descs[0]);           // closing the pipe for reading
	dup2(pipe_descs[1], STDOUT_FILENO);
	for (i = 0; i < LEN; i++) {
		num = rand() % (SIZE);
		start = time(NULL);
		search(arr, num);
		end = time(NULL);
		total_time += (int) end - (int) start;
	}	printf("%d", total_time / LEN); // prints to pipe
	close(pipe_descs[1]);           // closing the pipe for writing
}
//---------------------------------------------------------------------------//
// This function get an (unsorted) integers array.
// This a function of a child process which raffle a number 100 times and
// search if he exists in the array using binary search.
void child_d(int arr[], int pipe_descs[]) {
	int i,                  // index for running on the array's cells
			num,                // keep the number that raffled
			total_time = 0;      // the total time the searchs took (in seconds)
	time_t start,              // the time passed when the program started
			end;                // the time passed when the program ended
	close(pipe_descs[0]);           // closing the pipe for reading
	dup2(pipe_descs[1], STDOUT_FILENO);
	for (i = 0; i < LEN; i++) {
		num = rand() % (SIZE);
		start = time(NULL);
		binary_search(arr, 0, SIZE - 1, num);
		end = time(NULL);
		total_time += (int) end - (int) start;
	}printf("%d", total_time / LEN); // prints to pipe
	close(pipe_descs[1]);           // closing the pipe for writing
}
//---------------------------------------------------------------------------//
// This function get an integers array and an integer number.
// Performs a sequential search on the array.
void search(int arr[], int num) {
	int i;      // index for running on array
	for (i = 0; i < SIZE; i++)
		if (arr[i] == num)
			break;
	return;
}
//---------------------------------------------------------------------------//
// This function get an integers array
// Performs a Bubble-sort search on the array
void bubble_sort(int arr[]) {
	int i, j;           // indexes for running on array
	for (i = 0; i < SIZE - 1; i++) {
		// Last i elements are already in place
		for (j = 0; j < SIZE - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
	}}
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
		// If current element is smaller than or equal to pivot
		if (arr[j] <= pivot) {
			i++;                // increment index of smaller element
			swap(&arr[i], &arr[j]);
		}
	}	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}
//---------------------------------------------------------------------------//
// This function gets an array and a range by to numbers (=> [low, right])
// Performs the quick sort algorithm on this array
void quicksort(int array[], int low, int high) {
	if (low < high) {
		// pi is partitioning index, array[p] is now at right place
		int pi = partition(array, low, high);
		// Separately sort elements before partition and after partition
		quicksort(array, low, pi - 1);
		quicksort(array, pi + 1, high);
	}}
//---------------------------------------------------------------------------//
// This function gets an array, his size and a number.
// Performs a binary search on this array to find that number.
void binary_search(int arr[], int low, int size, int num) {
	while (low <= size) {
		int mid = low + (size - low) / 2;
		if (arr[mid] == num)     // Check if x is present at mid
			break;
		if (arr[mid] < num)     // If x greater, ignore left half
			low = mid + 1;
		else size = mid - 1; // If x is smaller, ignore right half
	}	return; // if we reach here, then element was not present
}
//---------------------------------------------------------------------------//
// This function gets a processes id.
// checks if the program can execute fork()
void can_fork(pid_t process) {
	if (process < 0) {
		perror("Cannot fork!\n");
		exit(EXIT_FAILURE);
	}
}
//---------------------------------------------------------------------------//
// This function gets a pipe (array descriptors)
// checks if the program can open the pipe
void open_pipes(int _pipes[]) {
	if (pipe(_pipes) == -1) {
		perror("cannot open pipe");
		exit(EXIT_FAILURE);
	}
}
