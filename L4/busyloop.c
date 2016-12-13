#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "busyloop.h"

int
busyLoop (int asec, int amsec)
{
	clock_t m1, m2;		// measure the real cpu time
	double elapse;
	m1 = clock ();

	int sec = asec;
	int milisec = amsec;

	if (sec > 10 & sec < 0)
		fprintf (stderr, "Invalid range of seconds");
	if (milisec > 999 & milisec < 0)
		fprintf (stderr, "Invalid range of miliseconds");

	struct timespec start, end;

	clock_gettime (CLOCK_MONOTONIC, &start);
	loop (sec, milisec);
	clock_gettime (CLOCK_MONOTONIC, &end);

	long diff =
		(end.tv_sec * BILLION + end.tv_nsec) -
		(start.tv_sec * BILLION + start.tv_nsec);

	m2 = clock ();
	elapse = round ((double) m2 - m1) / MSEC;
	return diff * MILLIONTH;

}


void
loop (long sec, long milisec)
{
	volatile int i;

	struct timespec begin, off;
	clock_gettime (CLOCK_MONOTONIC, &begin);
	for (i = 0;; ++i)
	{
		clock_gettime (CLOCK_MONOTONIC, &off);
		if (gtd (begin, off, (sec * BILLION + milisec * MILLION)) ==
		    1)
			break;
	}
}

int
gtd (struct timespec t1, struct timespec t2, long duration)
{
	if (((t2.tv_sec * BILLION + t2.tv_nsec) -
	     (t1.tv_sec * BILLION + t1.tv_nsec)) >= duration)
		return 1;
	return 0;
}
