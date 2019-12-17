/* 
 * Operating Systems {2INCO} Practical Assignment
 * Threaded Application
 *
 * Marwan ElAyouti (1282565)
 * Stefan Cracea ()
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

#include "uint128.h"
#include "flip.h"



//Mutex declaration and initialization.
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct 
{
    pthread_t id;
    int index;   
    int multiple_attr;
    bool active; 
    bool done;
} thread;

static thread threads[NROF_THREADS]; 


void intializeBuffer() {

	for (int i = 0; i < (NROF_PIECES/128) + 1; ++i)
	{
		buffer[i] = UINT128(0,0);
	}

}
void printResult() 
{

	for (int i = 0; i < NROF_PIECES; ++i)
	{
		if(BIT_IS_SET(buffer[i / 128], i % 128))
		{
			printf("%d\n", i);
		}
	}

}

static void * flipPieces(void * arg)
{
	int32_t multiple = * (int *) arg;
	free(arg);

	for (int i = multiple; i <= NROF_PIECES; i += multiple
		BIT_TOGGLE(buffer[i / 128], i % 128);
	}

}
int main (void)
{
    
    int multiple = 1;
    
    intializeBuffer();
   	
   	while(multiplem <= NROF_PIECES)
   		{
   			int * multiple_arg = malloc(sizeof (int));
   			*multiple_arg = multiple;

   			flipPieces(multiple_arg);

   			multiple++;
   		}

   	printResult();

    return (0);
}