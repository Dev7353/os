#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>

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
	
	printf("================= Get scheduler policy test =================\n");
	
	int policy = sched_getscheduler(pid);
	
	if(policy == SCHED_OTHER)
		printf("POLICY: the standard round-robin time-sharing policy\n");
	else if(policy == SCHED_RR)
		printf("POLICY: a round-robin policy.\n");
	else if(policy == SCHED_FIFO)
		printf("POLICY: a first-in, first-out policy\n");
	else
	{
		fprintf(stderr, "sched_getscheduler error occours.\n");
		printf("ERRNO: %d\n", errno);
		return -1;
	}
		
		
	printf("================= Set scheduler policy test =================\n");

	int ret = sched_setscheduler(0, SCHED_RR, &param);
	if(ret != 0)
	{
		fprintf(stderr, "cannot set scheduler\n");
		printf("ERRNO %d, retval: %d\n", errno, ret);
		return -1;
	}
	
	printf("=================  test =================\n");

	
	return 0;
}
