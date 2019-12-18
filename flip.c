/* 
 * Operating Systems {2INCO} Practical Assignment
 * Threaded Application
 *
 * Marwan ElAyouti (1282565)
 * Stefan Cracea (1231501)
 *
 * 
 * Grading:
 * Students who hand in clean code that fully satisfies the minimum requirements will get an 8. 
 * Extra steps can lead to higher marks because we want students to take the initiative. 
 * Extra steps can be, for example, in the form of measurements added to your code, a formal 
 * analysis of deadlock freeness etc.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>          // for perror()
#include <pthread.h>
#include <math.h>
#include "uint128.h"
#include "flip.h"

// create a bitmask where bit at position n is set
#define BITMASK(n)          (((uint128_t) 1) << (n))

// check if bit n in v is set
#define BIT_IS_SET(v,n)     (((v) & BITMASK(n)) == BITMASK(n))

// set bit n in v
#define BIT_SET(v,n)        ((v) =  (v) |  BITMASK(n))

// clear bit n in V
#define BIT_CLEAR(v,n)      ((v) =  (v) & ~BITMASK(n))

//Flips a bit n in V
#define BIT_XOR(v,n)      ((v) =  (v) ^ BITMASK(n))



//Mutex declaration and initialization.
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
static int nrThreadsActive = 0; //Number of threads in use.

/*Datastructure of a thread*/
typedef struct 
{
    /*Thread ID*/
    pthread_t id;
    
    /*Thread number*/
    int number;   
    
    /*Thread Multiple to work on*/
    int mult;


    /*Thread's State*/

    bool active; //True if active, False otherwise.

    bool done; //True if thread is done, False otherwise.

} thread;

/*Create a array of threads with size corresponding to max number of threads allowed*/
thread threads[NROF_THREADS]; 


/*Initializes the buffer to all ones meaning that all pieces are black.*/
void intializeBuffer()
{
	for (int i = 0; i < (NROF_PIECES/128 +1); ++i)
	{
		buffer[i] = ~0;
	}

}

/*Prints the final result.*/
void printResult() 
{	
	int j = 1;
	do 
	{	//print the number j whose representative bit is set to 1 
		//in the binary representation of the integer on position j/128 in the buffer
		//as long as j is strictly smaller than NROF_PIECES
		if(BIT_IS_SET(buffer[j / 128], j % 128))
		{
			printf("%d\n", j);
		}

		j ++;

	} while (j != NROF_PIECES);	

}


/*The task a thread does when it is created.*/
void * threadTask(void * arg);


/*Sets all threads to inactive*/
void prepareThreads(int maxthreads) 
{
	for (int i = 0; i < maxthreads ; i++) 
	{
		threads[i].active = false;
	}
}

/*Creates a thread assigns a multiple  and updates the flags that it is active and not done*/
void createThread(int i, int multiple) 
{
	threads[i].mult = multiple;
	threads[i].number = i;
	threads[i].done = false;
	threads[i].active = true;	
	pthread_create(&threads[i].id,NULL,threadTask,&threads[i].number); //create thread
}

/*Joins threads and sets them as inactive*/
void joinThreads(int j) 
{
 	pthread_join(threads[j].id, NULL);
 	threads[j].done = false;
 	threads[j].active = false;	
}

/*Joins all remaining active threads when our task is complete.*/
void killAllActiveThreads(int maxthreads)
{
	for (int i = 0; i < maxthreads; i++)
  	{
  		if (threads[i].active) {
  			pthread_join(threads[i].id,NULL);
  		}
  	}
}

/*If all threads are in use a lock is put and only release when a broadcast signal is recieved.*/
void waitForBroadcast()
{
	pthread_mutex_lock(&mutex);

  	while(nrThreadsActive + 1 > NROF_THREADS) 
  	{
  		pthread_cond_wait(&condition, &mutex);
  	}

  	pthread_mutex_unlock(&mutex);
}


int main (void)
{
	//Initialize the buffer
	intializeBuffer();
	
	//Initialize the mutex
	pthread_mutex_init(&mutex,NULL);
	
	//Prepare the threads to be createds
	prepareThreads(NROF_THREADS);	



    int threadCounter = 0;
	int multiple = 2; //Multiple variable that we will flip our bits with that will be passed to a thread.
	
	//Create NROF_THREADS
	while(threadCounter < NROF_THREADS && multiple <= NROF_PIECES )
	{ 
		createThread(threadCounter, multiple);
		nrThreadsActive++;
		threadCounter++;
		if (threadCounter < NROF_THREADS)
		{
			multiple++;
		}
	}

	//add new threads to handle the tasks as the old threads finish theirs
  	while(multiple <= NROF_PIECES) 
  	{
  		waitForBroadcast();
  		
  		
  		for (int j = 0; j < NROF_THREADS; ++j)
  		{
  			if(threads[j].done) 
  			{
  				joinThreads(j);
  				
  				multiple++;
  				
  				
  				if(multiple <= NROF_PIECES) 
  				{
  					createThread(j,multiple);
  					nrThreadsActive++;
  				}
  			}
  		}
  	}


  	/*By now all threads should be done with their task*/

  	killAllActiveThreads(NROF_THREADS);

  		   	
  	/*Print the final result*/
  	printResult();

  	//printf(" Total Thread count is %d: ", threadCount);
    
    return (0);
}

/*Flips every multiple of a buffer*/
void updateBuffer(int multiple) 
{
	for (int i = multiple; i < NROF_PIECES; i += multiple)
	{
		pthread_mutex_lock(&mutex);
		BIT_XOR(buffer[i / 128], i % 128);
		pthread_mutex_unlock(&mutex);
	}
}

void * threadTask(void * arg) 
{
	int number = * (int *) arg;
	int multiple = threads[number].mult;
	
	
	updateBuffer(multiple);
	

	threads[number].done = true; //Thread is done with its task
	pthread_mutex_lock(&mutex);
	nrThreadsActive --;
	pthread_cond_broadcast(&condition); 
	pthread_mutex_unlock(&mutex);
	return (0);
}
