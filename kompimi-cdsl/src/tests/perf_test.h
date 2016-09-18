/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#ifndef _PERF_TEST_

#include <time.h>

/* Example: time_elapsed("insert_end_dllist", insert_end_dllist, 20000000, (&dllst)); */
#define /*void*/ time_elapsed(/*char**/ name, /*int*/ iterations, body) \
    do \
    { \
	clock_t time_elapsed_begin_time = clock(); \
        clock_t time_elapsed_duration; \
	int time_elapsed_i; \
	for (time_elapsed_i=0; time_elapsed_i < (iterations); time_elapsed_i++) \
	{ \
	    body \
	} \
        time_elapsed_duration = clock() - time_elapsed_begin_time; \
	report_time((name), (iterations), time_elapsed_duration); \
    } while(0)
            
void report_time(char* name, int iterations, time_t clock_duration);

#endif /* #ifndef _PERF_TEST_ */
