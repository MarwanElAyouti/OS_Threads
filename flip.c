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
	{	
		if(BIT_IS_SET(buffer[j / 128], j % 128))
		{
			printf("%d\n", j);
		}

		j ++;

	} while (j != NROF_PIECES);	

}


// The actions a thread does when is available
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
	intializeBuffer();
	

	pthread_mutex_init(&mutex,NULL);
	
	prepareThreads(NROF_THREADS);	



    int threadCount = 0;
	int multiple = 2; //we start at 2
	
	while(multiple <= NROF_PIECES && threadCount < NROF_THREADS){ //start NROF_TREADS firsts threads
		createThread(threadCount, multiple);
		pthread_mutex_lock(&mutex);
		nrThreadsActive++;
		pthread_mutex_unlock(&mutex);
		threadCount++;
		if (threadCount < NROF_THREADS){
			multiple++;
		}
	}

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
  					threads[j].active = true;
  					threads[j].mult = multiple;
  					pthread_mutex_lock(&mutex);
  					nrThreadsActive++;
  					pthread_mutex_unlock(&mutex);
  					pthread_create(&threads[j].id, NULL, threadTask, &threads[j].number);
  				}
  			}
  		}
  	}

  	killAllActiveThreads(NROF_THREADS);

  		   	

  	printResult();

    return (0);
}

/*Flips every multiple of a buffer*/
void flipBuffer(int multiple) 
{
	for (int i = multiple; i < NROF_PIECES; i += multiple)
	{
		
		BIT_XOR(buffer[i / 128], i % 128);
		
	}
}

void * threadTask(void * arg) 
{
	int number = * (int *) arg;
	int multiple = threads[number].mult;
	
	pthread_mutex_lock(&mutex);
	flipBuffer(multiple);
	pthread_mutex_unlock(&mutex);

	threads[number].done = true; //Thread is done with its task
	pthread_mutex_lock(&mutex);
	nrThreadsActive --;
	pthread_cond_broadcast(&condition); 
	pthread_mutex_unlock(&mutex);
	return (0);
}
