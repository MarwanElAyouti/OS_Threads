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
<<<<<<< HEAD
=======

>>>>>>> 63e136ecacd17f2f2f02a96aec3aadf7912d06cd
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
	int32_t spacing = * (int *) arg;
	free(arg);

<<<<<<< HEAD
	for (int i = spacing; i <= NROF_PIECES; i += spacing)
	{
=======
	for (int i = multiple; i <= NROF_PIECES; i += multiple
>>>>>>> 63e136ecacd17f2f2f02a96aec3aadf7912d06cd
		BIT_TOGGLE(buffer[i / 128], i % 128);
	}

}
int main (void)
{
    
<<<<<<< HEAD
    int spacing = 1;
    
    intializeBuffer();
   	
   	while(spacing <= NROF_PIECES)
=======
    int multiple = 1;
    
    intializeBuffer();
   	
   	while(multiplem <= NROF_PIECES)
>>>>>>> 63e136ecacd17f2f2f02a96aec3aadf7912d06cd
   		{
   			int * multiple_arg = malloc(sizeof (int));
   			*multiple_arg = spacing;

   			flipPieces(multiple_arg);

   			spacing++;
   		}

   	printResult();

    return (0);
}