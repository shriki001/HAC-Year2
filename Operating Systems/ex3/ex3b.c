/******************************************************************************
 * Exr. #3b: This program created an array of raffled numbers and create N
 *           child processes, each one look at a part of the array cells, cells
 *           are by the reminder of the index in NUM_OF_CHILDERN is index.
 *           (For example: if there are 5 children ,the 1st child is in charge of
 *           looking at arr[1],arr[6],arr[11]... ,etc).
 *           all children sent the prime numbers they found to the father and
 *           the father sort these value, than prints and finish.
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
 * Output: The (prime) values that got from the children processes,at sorted
 *         order
 *
 * =============================================================================
 ******************************************************************************/
// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>         // for fork()
#include <stdbool.h>
#include <wait.h>
#include <math.h>
// ============================== Defines ================================== //
#define ARR_SIZE 100              // the size of the raffled numbers array
#define MAX_RAFFLE_NUM 50        // the highest number that can be raffled
#define NUM_OF_CHILDRENS 6      // number of process the father process births
// ============================= Prototypes ================================ //
void do_son_x(int num_of_son, int arr[], int pipe_descs[]);
void do_father(int pipe_descs[]);
bool is_prime(int num);
void bubble_sort(int arr[], int n);
void swap(int* a, int* b);
void insert_last(int value, int* *arr, int* arr_size);
// ================================ Main =================================== //
int main() {
	pid_t status;                   // keep the current process ID
	int index;                      // index for running on array
	int numbers_arr[ARR_SIZE];      // array of raffeld numbers
	int pipe_descs[2];              // defines an read-write pipe
	srand((unsigned) time(NULL));
	// insert raffled numbers into the array
	for (index = 0; index < ARR_SIZE; ++index)
		numbers_arr[index] = rand() % MAX_RAFFLE_NUM;
	// checking if we can open the pipe
	if (pipe(pipe_descs) == -1) {
		perror("Cannot open pipe");
		exit(EXIT_FAILURE);
	}
	// the father process births all childrens
	for (index = 0; index < NUM_OF_CHILDRENS; index++) {
		status = fork();
		switch (status) {
		case -1:
			perror("Cannot fork\n");
			exit(EXIT_FAILURE);
			// a particular child process case
		case 0:
			do_son_x(index, numbers_arr, pipe_descs);
			exit(EXIT_SUCCESS);
			// the father process case
		default:
			wait(0);
			break;
		}
	}
	do_father(pipe_descs);
	return EXIT_SUCCESS;
}
// =============================== Functions =============================== //
//----------------------------------------------------------------------------/
// This function in charge for each of child process(es) procedure
// It get the number of the son, the array we created ,and the pipe descriptors.
// Sends to the father process all the prime numbers of the part of array he has
// to check.
void do_son_x(int num_of_son, int array[], int pipe_descs[]) {
	int i;                        // index for running on array
	int* prime_arr = NULL;        // pointer for creating the prime-only array
	int prime_arr_size = 0;       // keep size of the prime-only array
	int curr_cell;              // value of the current cell we travel
	close(pipe_descs[0]);       // closes the pipe for writing
	// running on the cells the child is in charge of
	for (i = num_of_son; i < ARR_SIZE; i += NUM_OF_CHILDRENS) {
		curr_cell = array[i];
		// collect the primes of the child's part of the original array
		if (is_prime(curr_cell))
			insert_last(curr_cell, &prime_arr, &prime_arr_size);
	}
	// writes data of the prime-only array to the pipe
	write(pipe_descs[1], prime_arr, sizeof(int) * prime_arr_size);
	// closes the pipe for reading
	free(prime_arr);
	close(pipe_descs[1]);
}
//----------------------------------------------------------------------------/
// This function in charge of the father process procedure
// It get the pipe descriptors.
// Printing to the screen all the values that the childern(s) sent him
// through the pipe in an up-rising order.
void do_father(int pipe_descs[]) {
	int i;                      // index for running on array
	int buff[100] = { };           // buffer for reading from pipe
	int* array = NULL;          // array for keeping all the prime values
	int arr_size = 0;             // size of the created array
	// closing the pipe for writing, and reading the values from it
	close(pipe_descs[1]);
	read(pipe_descs[0], buff, sizeof(int) * 100);
	// reading from buffer and copying value into an array, for sorting
	for (i = 0; buff[i] != 0; ++i)
		insert_last(buff[i], &array, &arr_size);
	// sorting the array we built, and print him after sort.
	bubble_sort(array, arr_size);
	puts("sorted array: ");
	for (i = 0; i < arr_size; ++i)
		printf("%d,", array[i]);
	putchar('\n');
	free(array);
}
//----------------------------------------------------------------------------/
// This function get an integer number.
// Returns true if it a prime numbe, otherwise - returns false.
bool is_prime(int num) {
	int i;      // index for checking if the number is divided
	// one or zero doesn't count as primes
	if (num == 0 || num == 1)
		return false;
	// else we check if the num has at least on divider
	for (i = 2; i <= num / 2; i++)
		if (num % i == 0)
			return false;
	return true;
}
//----------------------------------------------------------------------------/
// This function gets an array of integers ant it's size (n)
// Performs a bubble-sort on this array, returns void.
void bubble_sort(int arr[], int n) {
	int i, j;
	// thg sort process
	for (i = 0; i < n - 1; i++)
		for (j = 0; j < n - i - 1; j++)
			if (arr[j] > arr[j + 1])
				swap(&arr[j], &arr[j + 1]);
}
//----------------------------------------------------------------------------/
// This function get 2 pointers to integers numbers
// Swaps between the values of each one.
void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}
//----------------------------------------------------------------------------/
// This function get a value, a dynamic array ant it's size
// Insert the value as the new (last) value on the array.
void insert_last(int value, int* *arr, int* arr_size) {
	(*arr) = realloc((*arr), sizeof(int) * ((*arr_size) + 1));
	(*arr)[*arr_size] = value;       // adding new item
	(*arr_size)++;                  // update size
}
