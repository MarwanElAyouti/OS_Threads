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

int main (void)
{
    // TODO: start threads to flip the pieces and output the results
    // (see thread_test() and thread_mutex_test() how to use threads and mutexes,
    //  see bit_test() how to manipulate bits in a large integer)

	for (int i = 0; i <= buffer.size(); i++) {
		buffer[i] = ~0;
	}
	printf ("buffer[0] (all 1's) : %lx%016lx\n", HI(buffer[0]), LO(buffer[0])); 
    return (0);
}
