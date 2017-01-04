#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <stdlib.h>

/*function prototypes*/
int linux_scheduler_test ();
int pthread_scheduler_test ();
void printHelp ();

int
main (int argc, char *argv[])
{

	int c = 0;
	while ((c = getopt (argc, argv, "hpl")) != -1)
	{
		switch (c)
		{
		case 'h':
			printHelp ();
			return 0;
		case 'l':
			return linux_scheduler_test ();
		case 'p':
			return pthread_scheduler_test ();
		case '?':
			break;
		}
	}
	return 0;
}


int
linux_scheduler_test ()
{
#ifdef __linux__

	pid_t pid = 0;		//mainthread
	struct sched_param param;
	errno = 0;

	printf ("[Get scheduler policy test]\n");
	int policy = sched_getscheduler (pid);
	if (policy == SCHED_OTHER)
		printf ("\tPOLICY: the standard round-robin time-sharing policy\n");
	else if (policy == SCHED_RR)
		printf ("\tPOLICY: a round-robin policy.\n");
	else if (policy == SCHED_FIFO)
		printf ("\tPOLICY: a first-in, first-out policy\n");
	else
	{
		fprintf (stderr, "\tsched_getscheduler error occours.\n");
		printf ("\tERRNO: %d\n", errno);
	}


	printf ("[Set scheduler policy test]\n");
	int ret = sched_setscheduler (0, SCHED_RR, &param);
	if (ret != 0)
	{
		fprintf (stderr, "\tcannot set scheduler\n");
		printf ("\tERRNO %d\n", errno);
	}
	else
		printf ("\tset scheduler policy successfully\n");

	printf ("[Get priority test]\n");
	errno = 0;
	int priority = getpriority (PRIO_PROCESS, 0);	// 0 is the calling process
	if (errno != 0)
	{
		printf ("\tcannot get thr priority\n");
		printf ("\tERRNO: %d\n", errno);
	}
	printf ("\tget priority successfully\n");

	printf ("[Set priority test]\n");
	priority = -19;		//highest prio
	if (setpriority (PRIO_PROCESS, 0, priority) != 0)
	{
		fprintf (stderr, "\tcannot set priority\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tset priority successfully");

	printf ("[Get param test]\n");
	ret = sched_getparam (pid, &param);
	if (ret != 0)
	{
		fprintf (stderr, "\tcannot get the scheduling parameter\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tget param successfully \n");

	printf ("[Set param test]\n");
	ret = sched_setparam (pid, &param);
	if (ret != 0)
	{
		fprintf (stderr, "\tcannot set param\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tset param successfully\n");

	printf ("[Get max scheduling priority of RR]\n");
	errno = 0;
	int max = sched_get_priority_max (SCHED_RR);
	if (errno != 0)
	{
		fprintf (stderr,
			 "\tcannot get the max scheduling priority\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tget the max priority successfully: %d\n", max);

	printf ("[Get min scheduling priority of RR]\n");
	errno = 0;
	int min = sched_get_priority_min (SCHED_RR);
	if (errno != 0)
	{
		fprintf (stderr,
			 "\tcannot get the min scheduling priority\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tget the min priority successfully: %d\n", min);


	printf ("[Get RR interval test]\n");
	errno = 0;
	struct timespec iv;
	ret = sched_rr_get_interval (pid, &iv);
	if (ret != 0)
	{
		fprintf (stderr, "\tcannot get rr intervall\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tsuccessfully call, intervall is %ld s %lu ns\n",
			iv.tv_sec, iv.tv_nsec);

#endif
	return 0;
}

int
pthread_scheduler_test ()
{
	printf ("[Get scheduling parameter test]\n");
	pthread_attr_t attr;
	struct sched_param param;
	errno = 0;
	if (pthread_attr_getschedparam (&attr, &param) != 0)
	{
		fprintf (stderr, "\tcannt get the scheduling parameter\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tgot successfully the scheduling parameter\n");

	printf ("[Set scheduling parameter test]\n");
	errno = 0;
	if (pthread_attr_getschedparam (&attr, &param) != 0)
	{
		fprintf (stderr, "\tcannt set the scheduling parameter\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tset successfully the scheduling parameter\n");

	printf ("[Get policy test]\n");
	errno = 0;
	int policy = 0;
	if (pthread_attr_getschedpolicy (&attr, &policy) != 0)
	{
		fprintf (stderr, "\tcannot get the scheduling policy\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tsuccessfully get the scheduling policy\n");

	printf ("[Set policy test]\n");
	errno = 0;
	if (pthread_attr_setschedpolicy (&attr, SCHED_OTHER) != 0)
	{
		fprintf (stderr, "\tcannot set the scheduling policy\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tsuccessfully set the schedling policy\n");

	printf ("[Get inherit scheduler attribute test]\n");
	errno = 0;
	int inherit = 0;
	if (pthread_attr_getinheritsched (&attr, &inherit) != 0)
	{
		fprintf (stderr, "\tcannot get the inherit scheduler info\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tsuccessfully get the inherit scheduler info\n");

	printf ("[Set inherit scheduler attribute test]\n");
	errno = 0;
	if (pthread_attr_setinheritsched (&attr, PTHREAD_INHERIT_SCHED) != 0)
	{
		fprintf (stderr, "\tcannot set the inherit scheduler\n");
		printf ("\tERRNO: %d\n", errno);
	}
	else
		printf ("\tsuccessfully set the inherit scheduler\n");
	return 0;
}

void
printHelp ()
{
	printf ("Help\n");
	printf ("-h --help \t\t Help\n");
	printf ("-p --pthread api \t starts tests for pthread scheduling api\n");
	printf ("-l --linux thread api \t starts tests for linux scheduling api. Only Linux support.\n");
	printf ("============================================================\n");
	printf ("Current Git HEAD commit number: \n");
	const char *gitversion = "git rev-parse HEAD";
	system (gitversion);
}
