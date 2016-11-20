#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BILLION 1000000000
#define MILLION 1000000
#define MILLIONTH 0.000001

void loop(long sec, long milisec);
int gtd(struct timespec t1, struct timespec t2, long duration);

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		fprintf(stderr, "To less or to many arguments");
	}

	int sec = atoi(argv[1]);
	int milisec = atoi(argv[2]);
	
	if(sec > 10 & sec < 0)
		fprintf(stderr, "Invalid range of seconds");
	if(milisec > 999 & milisec < 0)
		fprintf(stderr, "Invalid range of miliseconds");
	
	struct timespec start, end;
	
	clock_gettime(CLOCK_MONOTONIC, &start);
	loop(sec, milisec);
	clock_gettime(CLOCK_MONOTONIC, &end);
		
	long diff = (end.tv_sec*BILLION + end.tv_nsec) - (start.tv_sec*BILLION + start.tv_nsec);

	printf("execution time (in ms): %ld\n", (long) (diff*MILLIONTH));
	return 0;
	
} 


void loop(long sec, long milisec)
{
	volatile int i;
	
	struct timespec begin, off;
	clock_gettime(CLOCK_MONOTONIC, &begin);	
	for(i = 0;;++i)
	{
		clock_gettime(CLOCK_MONOTONIC, &off);
		if(gtd(begin, off, (sec*BILLION + milisec*MILLION)) == 1)
			break;
	}
}

int gtd(struct timespec t1, struct timespec t2, long duration)
{
	if(((t2.tv_sec*BILLION + t2.tv_nsec) - (t1.tv_sec*BILLION + t1.tv_nsec)) >= duration)
		return 1;
	return 0;
}
