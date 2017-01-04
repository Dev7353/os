#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>

/*function prototypes*/
int linux_scheduler_test();

int main(int argc, char* argv[])
{

	int c = 0;
	while ((c = getopt (argc, argv, "hpl")) != -1)
	{
		switch (c)
		{
		case 'h':
			printf("Help\n");
			return 0;
		case 'l':
			return linux_scheduler_test();
		case '?':
			break;
		}
	}
	return 0;
}


int linux_scheduler_test()
{
	pid_t pid = 0; //mainthread
	struct sched_param param;
	errno = 0;
	
	printf("[Get scheduler policy test]\n");
	int policy = sched_getscheduler(pid);
	if(policy == SCHED_OTHER)
		printf("\tPOLICY: the standard round-robin time-sharing policy\n");
	else if(policy == SCHED_RR)
		printf("\tPOLICY: a round-robin policy.\n");
	else if(policy == SCHED_FIFO)
		printf("\tPOLICY: a first-in, first-out policy\n");
	else
	{
		fprintf(stderr, "\tsched_getscheduler error occours.\n");
		printf("\tERRNO: %d\n", errno);
	}
		
		
	printf("[Set scheduler policy test]\n");
	int ret = sched_setscheduler(0, SCHED_RR, &param);
	if(ret != 0)
	{
		fprintf(stderr, "\tcannot set scheduler\n");
		printf("\tERRNO %d\n", errno);
	} else
		printf("\tset scheduler policy successfully\n");
	
	printf("[Get priority test]\n");
	errno = 0;
	int priority = 	getpriority(PRIO_PROCESS, 0); // 0 is the calling process
	if(errno != 0)
	{
		printf("\tcannot get thr priority\n");
		printf("\tERRNO: %d\n", errno);
	}
	printf("\tget priority successfully\n");
	
	printf("[Set priority test]\n");
	priority = -19; //highest prio
	if(setpriority(PRIO_PROCESS, 0, priority) != 0)
	{
		fprintf(stderr, "\tcannot set priority\n");
		printf("\tERRNO: %d\n", errno);
	} else
		printf("\tset priority successfully");
	
	printf("[Get param test]\n");
	ret = sched_getparam(pid, &param);
	if(ret != 0)
	{
		fprintf(stderr, "\tcannot get the scheduling parameter\n");
		printf("\tERRNO: %d\n", errno);
	} else
		printf("\tget param successfully \n");
	
	printf("[Set param test]\n");
	ret = sched_setparam(pid, &param);
	if(ret != 0)
	{
		fprintf(stderr, "\tcannot set param\n");
		printf("\tERRNO: %d\n", errno);
	}else
		printf("\tset param successfully\n");
		
	printf("[Get max scheduling priority of RR]\n");
	errno = 0;
	int max = sched_get_priority_max(SCHED_RR);
	if(errno != 0)
	{
		fprintf(stderr, "\tcannot get the max scheduling priority\n");
		printf("\tERRNO: %d\n", errno);
	}else
		printf("\tget the max priority successfully: %d\n", max);
	
	printf("[Get min scheduling priority of RR]\n");
	errno = 0;
	int min = sched_get_priority_min(SCHED_RR);
	if(errno != 0)
	{
		fprintf(stderr, "\tcannot get the min scheduling priority\n");
		printf("\tERRNO: %d\n", errno);
	}else
		printf("\tget the min priority successfully: %d\n", min);
	
	
	printf("[Get RR interval test]\n");
	errno = 0;
	struct timespec iv;
	ret = sched_rr_get_interval(pid, &iv);
	if(ret != 0)
	{
		fprintf(stderr, "\tcannot get rr intervall\n");
		printf("\tERRNO: %d\n", errno);
	}else
		printf("\tsuccessfully call, intervall is %ld s %lu ns\n", iv.tv_sec, iv.tv_nsec);
	return 0;
}
