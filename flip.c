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



//Mutex declaration and initialization.
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct 
{
    pthread_t id;
    int number;   
    int mult;
    int state; //1 = "active", 2 = "waiting", 3 = "done"
} thread;


static thread threads[NROF_THREADS]; 


void intializeBuffer() 
{
	int i = 0;

	do 
	{
		buffer[i] = UINT128(0,0);
		i ++;
	} while (i != ceil(NROF_PIECES/128));

}

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
	int32_t multiple = * (int *) arg;
	free(arg);

	for (int i = multiple; i <= NROF_PIECES; i += multiple)
	{

		BIT_TOGGLE(buffer[i / 128], i % 128);
	}

}


int main (void)
{

    intializeBuffer();

   	//Create the first NROF_THREADS threads

   	// for (int k = 0; k < NROF_THREADS; k ++) {
   	// 		threads[k].multiple_attr = mult;
   	// 		threads[k].state_thread = 1;
   	// 		pthread_create(&threads[k].id, NULL, threadTask, &k); 
   	// 		pthread_mutex_lock(&mutex);
   	// 		pthread_mutex_unlock(&mutex);
   	// }

   	for (int mult = 1; mult<= NROF_PIECES; mult ++) {

   		int * multiple_arg = malloc(sizeof (int));
   		*multiple_arg = mult;

   		threadTask(multiple_arg);
   	}

   	printResult();

    return (0);
}
