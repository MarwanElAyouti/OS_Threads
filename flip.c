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

// clear bit n in 
#define BIT_CLEAR(v,n)      ((v) =  (v) & ~BITMASK(n))

//
#define BIT_TOGGLE(v,n)      ((v) =  (v) ^ BITMASK(n))



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
	int i = 0;

	do 
	{
		buffer[i] = UINT128(1,1);
		i ++;
	} while (i != ceil(NROF_PIECES/128));

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
void * threadTask(void * arg) 
{
	int number = * (int *) arg;
	int multiple = threads[number].mult;
	

	for (int i = multiple; i <= NROF_PIECES; i += multiple)
	{

		BIT_TOGGLE(buffer[i / 128], i % 128);
	}

	threads[number].done = true; //Thread is done with its task
	pthread_mutex_lock(&mutex);
	nrThreadsActive --;
	pthread_cond_signal(&condition); 
	pthread_mutex_unlock(&mutex);
	return 0;
}


int main (void)
{
	

	pthread_mutex_init(&mutex,NULL);
	for (int i = 0; i < NROF_THREADS ; i++) 
	{
		threads[i].active = false;
	}

	int multiple = 1;


    intializeBuffer();

    
   	//Create the first NROF_THREADS threads

   	// for (int k = 0; k < NROF_THREADS; k ++) {
   	// 		threads[k].multiple_attr = mult;
   	// 		threads[k].state_thread = 1;
   	// 		pthread_create(&threads[k].id, NULL, threadTask, &k); 
   	// 		pthread_mutex_lock(&mutex);
   	// 		pthread_mutex_unlock(&mutex);
   	// }

    int index = 0;
  	while(multiple <= NROF_PIECES) 
  	{
  		
  		if(index < NROF_THREADS)
  		{
  			threads[index].number = index;
  			threads[index].active = true;
  			threads[index].done = false;
  			threads[index].mult = multiple;
  			pthread_create(&threads[index].id,NULL,threadTask,&threads[index].number);
  			printf("Thread created\n");
  			pthread_mutex_lock(&mutex);
			nrThreadsActive++;
			pthread_mutex_unlock(&mutex);
			multiple++;
			index++;

  		}

  		pthread_mutex_lock(&mutex);

  		while(nrThreadsActive + 1 > NROF_THREADS) 
  		{
  			pthread_cond_wait(&condition, &mutex);
  		}

  		pthread_mutex_unlock(&mutex);
  		
  		for (int j = 0; j < NROF_THREADS; ++j)
  		{
  			if(threads[j].done == true) 
  			{
  				threads[j].done = false;
  				threads[j].active = false;
  				pthread_join(threads[j].id, NULL);
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

   		for (int i = 0; i < NROF_THREADS; ++i)
   		{
   			if(threads[i].active == true) {
   				pthread_join(threads[i].id, NULL);
   			}
   		}

   	}

   	printResult();

    return (0);
}
