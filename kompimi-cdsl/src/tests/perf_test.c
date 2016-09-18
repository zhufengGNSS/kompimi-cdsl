/* Originally distributed as part of the Kompimi C Data Structure Library.
   For updates, see: https://sourceforge.net/projects/kompimi-cdsl/

   All content in this document is granted into the public
   domain. Where this is not legally possible, the copyright owner
   releases all rights. This notice may be modified or removed.
*/

#include <stdio.h>

#include "perf_test.h"

void report_time(char* name, int iterations, time_t clock_duration)
{
    double seconds_per_iteration =
	(((double)clock_duration)/CLOCKS_PER_SEC)/iterations;
    if (seconds_per_iteration < 1E-6)
    {
	printf("%s: %f ns per iteration\n", name, seconds_per_iteration*1E9);
    }
    else if (seconds_per_iteration < 1E-3)
    {
	printf("%s: %f us per iteration\n", name, seconds_per_iteration*1E6);
    }
    else if (seconds_per_iteration < 1E0)
    {
	printf("%s: %f ms per iteration\n", name, seconds_per_iteration*1E3);
    }
    else if (seconds_per_iteration < 60)
    {
	printf("%s: %f s per iteration\n", name, seconds_per_iteration);
    }
    else
    {
	printf("%s: %f min per iteration\n", name, seconds_per_iteration/60);
    }
}
