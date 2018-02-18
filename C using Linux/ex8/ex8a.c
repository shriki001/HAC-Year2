/******************************************************************************
* Exr. #8a: This Program does the following:
*           defines array of 100 integers, all celles are in range 0 to 10,
*           when 0 symboles empty cell.
*           the main thread creates 6 semi-threads:
*           - The first 3 ones:
*             Each one raffles a number in range (-10,10),
*             if the raffled number is positive, it inserts it into the array
*             (as long as have space).
*             if the raffled number is negetive, it 'empty' his negetive
*             from the array (mean it put 0 in his cell).
*             Each one, counts how many numbers inserted an how much emptied.
*
*           - The other 3 ones:
*             Each one raffles 1000 times a number in range 1 to 10, and
*             checking if it exist in array. finally return how much from
*             the values he searched are found in array
*
*            The main thread, after creating all (6) semi-threads, waiting
*            for them , and for each semi-thread presents the data it
*            returned from him.
*
* =============================================================================
* Written by  	: Kfir Matityahu & Michael Shriki
* ID	 	   	: 205425960 / 305599417
* login	   	    : kfirma / michaelshr
* Course/Year   : OS1 2018
* Year	   		: B
*
* =============================================================================
* Input: Nothing.
*
* -----------------------------------------------------------------------------
* Output: The data return from each (semi) thread.
*
* =============================================================================
******************************************************************************/

// ============================== Include ================================== //
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

// ============================== Defines ================================== //
#define SIZE 100                // size of the numbers' array
#define THREAD_ARR 6            // number of threads in program

// ============================= Prototypes ================================ //
void *group1(void *);
void *group2(void *);

// ================================ Main =================================== //
int main()
{
    int arr[SIZE] = {0};        // initialize the array to zeros
    pthread_t thread_data[THREAD_ARR];  // array keeps the data return from
                                        // each thread
    int status,             // keep current thread number
        i,                  // index for running on array
        *res;               // keep result

    srand((unsigned)time(NULL));

    // create and execute the semi-threads
    for (i = 0; i < THREAD_ARR; i++) {
        if (i < 3)      // thread is belong to the first group
            status = pthread_create(&(thread_data[i]), NULL, group1,
                                    (void *) arr);
        else  // thread is belong to the second group ,cells [3]-[5]
            status = pthread_create(&(thread_data[i]), NULL, group2,
                                    (void *) arr);

        if (status != 0)        // failed to create the thread
        {
            fputs("thread create failed in main", stderr);
            exit(EXIT_FAILURE);
        }
    }

    // create and excute the other 3 semi-threads
    for (i = 0; i < THREAD_ARR; i++) {
        pthread_join(thread_data[i], (void **) &res);

        if (i < 3)          // thread is belong to the first group
            printf("thread #%d\nadded : %d, deleted : %d \n", i + 1, res[0],
                   res[1]);
        else     // thread is belong to the seconds group
            printf("thread #%d\ncount : %d \n", i + 1, *res);

        free(res);
    }

    return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
// This function performs the method for the first group threads
// Gets a generic array, and counts how much numbers inserted and removed
// return those numbers to the the main thread
void *group1(void *array)
{
    int *arr = (int *) array;           // cast back to array of integers
    // keeps: in [0] the numbers inserted, in [1] the numbers removed
    int *ret_val = (int *) malloc(sizeof(int) * 2);
    int i,              // index for running on the array
        add = 0,        // counter of added numbers
        del = 0,        // counter of 'deleted' numbers
        num,            // keep the the number that raffled
        count = 0;

    while (count < SIZE * 10) {
        num = (rand() % 21) - 10;          // raffle a number in range (-10,10)
        // we raffled a positive number, therefore we add it to the array
        if (num > 0) {
            for (i = 0; i < SIZE; i++)
                if (arr[i] == 0) {
                    arr[i] = num;
                    add++;
                    break;
                }
        }

        // we raffled a negative number, therefore we 'clean' his negetive one
        // from the array
        else if (num < 0) {
            for (i = 0; i < SIZE; i++)
                if ((arr[i] + num) == 0) {
                    arr[i] = 0;
                    del++;
                    break;
                }
        }
        count++;
    }

    // update results and close from thread
    ret_val[0] = add;
    ret_val[1] = del;
    pthread_exit(ret_val);
}

//---------------------------------------------------------------------------//
// This function performs the method for the first group threads
// Gets a generic array, counts how much values he found
// return this value to the the main thread
void *group2(void *array)
{
    int i = 0, j;                   // indexes for running on array
    int count = 0,             // cast back to array of integers
        num;                    // keep the the number that raffled
    int *arr = (int *) array;
    int *ret_val = (int *) malloc(sizeof(int));


    while (i < SIZE * 10) {
        num = (rand() % 9) + 1;

        for (j = 0; j < SIZE; j++)
            if (arr[j] == num) {
                count++;
                break;
            }
        i++;
    }
    // update results and close from thread
    *ret_val = count;
    pthread_exit(ret_val);
}
