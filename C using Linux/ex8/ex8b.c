/******************************************************************************
* Exr. #8b: This Program does the following:
*           the main thread creates 5 threads.
*           Each thread runs in an endless loop , raffles a natrual number and
*           checks if it a prime.
*           if it's a prime, this thread cancels all the other (4) threads
*           and presets the number which raffled.
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
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>

// ============================== Defines ================================== //
#define SIZE 5                  // number of threads in program
#define RANGE 1000              // the range of number to be raffeld

// ============================= Prototypes ================================ //
void *my_func();
void kill_all_threads(void);
void init_key(void);
void cleanup_malloc(void *arg);
bool is_prime(int num);

// ============================== Globals ================================== //
pthread_t thread_data[SIZE];                       // array of threads ids
pthread_once_t threads_kill = PTHREAD_ONCE_INIT;   // initialize behaivor
pthread_once_t threads_init = PTHREAD_ONCE_INIT;
pthread_key_t key;                                 // the value thread return

// ================================ Main =================================== //
int main()
{
    int status,             // keep current thread id
        i;                  // index for running on array

    srand((unsigned)time(NULL));

    // creates the 5 threads
    for (i = 0; i < SIZE; i++) {
        status = pthread_create(&(thread_data[i]), NULL, my_func, NULL);

        if (status != 0) {  // failed to create
            fputs("thread create failed in  main", stderr);
            exit(EXIT_FAILURE);
        }
    }

    // main thread wait for all others threads
    for (i = 0; i < SIZE; i++)
        pthread_join(thread_data[i], NULL);

    return EXIT_SUCCESS;
}

// ============================= Functions ================================= //
// The main func of each thread
// Raffles a number and check if it's prime. if so - cancels all other threads
void *my_func()
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    pthread_once(&threads_init, init_key);
    int *num = (int *) malloc(sizeof(int));

    pthread_cleanup_push(cleanup_malloc, (void *) num) ;

    while (1) {
        *num = rand() % RANGE;
        if (is_prime(*num))
        {
            if (pthread_setspecific(key , num))
            {
                fputs("pthread_setspecific failed" , stderr);
                exit(EXIT_FAILURE);
            }
            pthread_once(&threads_kill , kill_all_threads);
            pthread_exit(NULL);
        }
    }
    pthread_cleanup_pop(0);
}

//---------------------------------------------------------------------------//
// This function get an integer number
// Return true if it prime number. otherwise - return false.
bool is_prime(int num)
{
    int div;         // index for running on array

    if (num == 1 || num == 0)
        return false;

    for (div = 2; div <= num / 2; div++)
        if (num % div == 0)
            return false;

    return true;
}

//---------------------------------------------------------------------------//
// This function 'kiils' all other running threads
void kill_all_threads()
{
    int i;                    // index for running on array
    pthread_t current;        // keep thread id

    current = pthread_self(); // set to current thread id

    // kill the other threads
    for (i = 0; i < SIZE; i++) {
        if (thread_data[i] != current) {
            pthread_cancel(thread_data[i]);
        }
    }
    int *prime_num = (int *) pthread_getspecific(key);

    printf("the Prime: %d\n", *prime_num);   // print the number
}

//------------------------------------------------------------------------------
// Init thread key
// This function check if failed to create pthread key
void init_key()
{
    if (pthread_key_create(&key, NULL)) {
        fputs("pthread_key_create failed", stderr);
        exit(EXIT_FAILURE);
    }
}

//------------------------------------------------------------------------------
//Cleanup method of malloc
void cleanup_malloc(void *arg)
{
    free((char *) arg);
}
