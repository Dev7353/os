#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <string.h>
#include "scheduler-api.h"
#include "time-api.h"

/*define function prototypes incl thread functions*/
void scheduler(void* arg);
void eat(void* arg);

int main(int argc, char* argv[])
{
	verbose = false;
	file = false;
	//Default amount of animals
	int cn = 6;
	int dn = 4;
	int mn = 2;
	
	
	//Default satisfied time
	int ct = 15;
	int dt = 10;
	int mt = 1;
	
	//Default amount the animals want to eat
	int ce = 5;
	int de = 5;
	int me = 5;

	int e = 1;
	int E = 1;
	
	int num_dishes = 2;
	
	int c = 0;
	while(1)
	{
		struct option long_options[] = 
		{
			{"v", 	no_argument, 		&verbose, 	true},
			{"file", 	required_argument,	0,		'f'},
			{"help", 	no_argument,		0,		'h'},
			{"cn", 		required_argument,	0,		'a'},
			{"dn", 		required_argument,	0,		'b'},
			{"mn", 		required_argument,	0,		'c'},
			{"ct", 		required_argument,	0,		'd'},
			{"dt", 		required_argument,	0,		'g'},
			{"mt", 		required_argument,	0,		'i'},
			{"ce", 		required_argument,	0,		'j'},
			{"de", 		required_argument,	0,		'k'},
			{"me", 		required_argument,	0,		'l'},
			{"e", 		required_argument,	0,		'm'},
			{"E", 		required_argument,	0,		'n'},
			{"dish",	required_argument,	0,		'o'}
			
		};
		
		int option_index = 0;
		
		c = getopt_long(argc, argv, "fh", long_options, &option_index);
		
		if(c == -1)
			break;
		switch(c)
		{
			case 0:
				if (long_options[option_index].flag != 0)
				    break;
				printf ("option %s", long_options[option_index].name);
				if (optarg)
					printf (" with arg %s", optarg);
				printf ("\n");
				break;
			case 'h':
				printHelp();
				return 0;
			case 'f':
				filename = optarg;
				file = true;
				break;
			case 'a':
			cn = atoi(optarg);
				break;
			case 'b':
			dn = atoi(optarg);
				break;
			case 'c':
			mn = atoi(optarg);
				break;
			case 'd':
			ct = atoi(optarg);
				break;
			case 'g':
			dt = atoi(optarg);
				break;
			case 'i':
			mt = atoi(optarg);
				break;
			case 'j':
			ce = atoi(optarg);
				break;
			case 'k':
			de = atoi(optarg);
				break;
			case 'l':
			me = atoi(optarg);
				break;
			case 'm':
			e = atoi(optarg);
				break;
			case 'n':
			E = atoi(optarg);
				break;
			case 'o':
			num_dishes = atoi(optarg);
				break;
			case '?':
				break;
			default:
				abort();
			
		}
		
	}
	
	/*define variables for threads*/
	pthread_t cats[cn];
	pthread_t dogs[dn];
	pthread_t mice[mn];
	pthread_t schedule;
	
	assert(pthread_mutex_init(&mutex, NULL) == 0);
	
	/*allocate storage for cvs*/
	cond_cats = (pthread_cond_t*) malloc(cn * sizeof(pthread_cond_t));
	assert(cond_cats != NULL);
	cond_dogs = (pthread_cond_t*) malloc(dn * sizeof(pthread_cond_t));
	assert(cond_dogs != NULL);
	cond_mice = (pthread_cond_t*) malloc(mn * sizeof(pthread_cond_t));
	assert(cond_mice != NULL);
	
	//initialize the priority queue struct
	prio.container = (pthread_cond_t**) malloc(GROUPS * sizeof(pthread_cond_t*));
	assert(prio.container != NULL);
	prio.container[0] = cond_cats;
	prio.container[1] = cond_dogs;
	prio.container[2] = cond_mice;
	
	prio.threads_per_group = (int*) malloc(GROUPS * sizeof(int));
	assert(prio.threads_per_group != NULL);
	prio.threads_per_group[0] = cn;
	prio.threads_per_group[1] = dn;
	prio.threads_per_group[2] = mn;
	
	prio.group_priority = (int*) calloc(GROUPS, sizeof(int));
	assert(prio.group_priority != NULL);
	prio.group_priority[0] = ct;
	prio.group_priority[1] = dt;
	prio.group_priority[2] = mt;
	
	prio.priority = (int**) malloc(3 * sizeof(int));
	assert(prio.priority != NULL);
	prio.priority[0] = (int*) calloc(cn, sizeof(int));
	assert(prio.priority[0] != NULL);
	prio.priority[1] = (int*) calloc(dn, sizeof(int));
	assert(prio.priority[1] != NULL);
	prio.priority[2] = (int*) calloc(mn, sizeof(int));
	assert(prio.priority[2] != NULL);
	
	//initialize arguments for threads
	animal_t* cat_args = (animal_t*) malloc(cn * sizeof(animal_t));
	assert(cat_args != NULL);
	animal_t* dog_args = (animal_t*) malloc(dn * sizeof(animal_t));
	assert(dog_args != NULL);
	animal_t* mouse_args = (animal_t*) malloc(mn * sizeof(animal_t));
	assert(mouse_args != NULL);
	
	//initialize food area, synchronize, threadDone 
	area.bowles = num_dishes;
	initializeGlobals();
	
	time_t sec;
	time(&sec);
	srandom ((unsigned int) sec);
	
	/*start threads*/
	assert(pthread_create(&schedule, NULL, (void *(*)(void *))&scheduler, NULL) == 0);
	
	for(int i = 0; i < cn; ++i)
	{
		assert(pthread_cond_init(&cond_cats[i], NULL) == 0);
		cat_args[i].num_eat = ce;
		cat_args[i].eating_time = random () % (E + 1 - e) + e;
		cat_args[i].satisfied_time = ct;
		cat_args[i].animal_type = CAT;
		cat_args[i].id = i;
		assert(pthread_create(&cats[i], NULL, (void *(*)(void *))&eat, &cat_args[i]) == 0);
		
	}
	
	for(int i = 0; i < dn; ++i)
	{
		assert(pthread_cond_init(&cond_dogs[i], NULL) == 0);
		dog_args[i].num_eat = de;
		dog_args[i].eating_time = random () % (E + 1 - e) + e;
		dog_args[i].satisfied_time = dt;
		dog_args[i].animal_type = DOG;
		dog_args[i].id = i;
		assert(pthread_create(&dogs[i], NULL, (void *(*)(void *))&eat, &dog_args[i]) == 0);

	}
		
	for(int i = 0; i < mn; ++i)
	{
		assert(pthread_cond_init(&cond_mice[i], NULL) == 0);
		mouse_args[i].num_eat = me;
		mouse_args[i].eating_time = random () % (E + 1 - e) + e;
		mouse_args[i].satisfied_time = mt;
		mouse_args[i].animal_type = MOUSE;
		mouse_args[i].id = i;
		assert(pthread_create(&mice[i], NULL, (void *(*)(void *))&eat, &mouse_args[i]) == 0);

	}
	
	/*join threads*/

	for(int i = 0; i < cn; ++i)
	{
		assert(pthread_join(cats[i], NULL) == 0);
		
	}
	
	for(int i = 0; i < dn; ++i)
	{
		assert(pthread_join(dogs[i], NULL) == 0);

	}
		
	for(int i = 0; i < mn; ++i)
	{
		assert(pthread_join(mice[i], NULL) == 0);

	}

	assert(pthread_join(schedule, NULL) == 0);
	
	free(area.status);
	
	free(cond_cats);
	free(cond_dogs);
	free(cond_mice);
}

void eat(void* arg)
{	time_t begin, end;
	
	char* thread_color = ANSI_COLOR_RESET;
	int animal = 0;
	animal_t param = *((animal_t*)arg);
	
	while(param.num_eat > 0)
	{
		/*animal gets hungry*/
		sleep(param.satisfied_time);
		
		begin = clock();
		
		if(verbose == true)
			printf("%s %d is hungry\n", param.animal_type, param.id);
		
		if(strcmp(param.animal_type, CAT) == 0)
		{
			thread_color = ANSI_COLOR_RED;
			isReady[animal]++;
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_cats[param.id], &mutex);
			pthread_mutex_unlock(&mutex);
		}
		else if(strcmp(param.animal_type, DOG) == 0)
		{
			thread_color = ANSI_COLOR_BLUE;
			animal = 1;
			isReady[animal]++;
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_dogs[param.id], &mutex);
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			animal = 2;
			isReady[animal]++;
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_mice[param.id], &mutex);
			pthread_mutex_unlock(&mutex);
		}
		
		end = clock();
		
		double diff = ((double) end-begin)/CLOCKS_PER_SEC;
		waiting_times[animal][nextTimeSlot(animal)] = diff;
		
		
		if(param.num_eat == 0)
			continue;
		
		//occupie dish with current animal
		int my_dish = nextBowle(area.status, area.bowles); 
		area.status[my_dish] = param.animal_type[0];
		
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		 printf("%s\t[%d-%d-%d %d:%d:%d] \t[%s] %s %d \tstarted eating from dish %d%s\n", thread_color, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, area.status, param.animal_type, param.id, my_dish, ANSI_COLOR_RESET);
		
		// decrements local number of eating times
		--param.num_eat; 

		while(true)
		{
			if(nextBowle(area.status, area.bowles) > -1)
			{
				break; 
			}
			continue;
		}
		
		synchronize[animal][param.id] = true;
		sleep(param.eating_time);

		//decremtn global variable so that scheduler releases only
		--isReady[animal];
		
		//get current date time
		t = time(NULL);
		tm = *localtime(&t);
		
		//clear the dish occupant 
		area.status[my_dish] = '-';
		printf("%s\t[%d-%d-%d %d:%d:%d] \t[%s] %s %d \tfinished eating from dish %d%s\n", thread_color, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec, area.status, param.animal_type, param.id, my_dish, ANSI_COLOR_RESET);
		
		
		synchronize[animal][param.id] = false;

	}
	
	threadDone[animal][param.id] = true;
	if(verbose == true)
		printf("%s%d %s is done%s\n", ANSI_COLOR_YELLOW, param.id, param.animal_type, ANSI_COLOR_RESET);
}

void scheduler(void* arg)
{
	int cnt = 0;
	time_t begin, end;
	while(true)
	{	
		if(verbose == true)
			printf("%s--------------------------------------------- Scheduler starts round number %d%s\n", ANSI_COLOR_GREEN, cnt, ANSI_COLOR_RESET);
			
		int animal = nextGroup();
		
		//check if all groups are done with eating
		if(animal == -1)
			break; 
		
		while(true)
		{
			if(isReady[animal] == prio.threads_per_group[animal] && prio.threads_per_group[animal] >= area.bowles)
			{
				break;
			} 
			//if you have less threads than number of bowles
			else if(isReady[animal] == prio.threads_per_group[animal] && prio.threads_per_group[animal] <= area.bowles) 
			{
				break;
			}
			
			continue;
		}
		if(verbose == true)
		{
			printf(">> RELEASE THREADS <<\n");
			printf(">>%d Threads are ready <<\n", isReady[animal]);
		}
		//THREAD RELEASE
		for(int j = 0; j < prio.threads_per_group[animal]; ++j)
		{
			begin = clock();
			pthread_cond_signal(nextAnimal(animal));
			while(true)
			{	
				if(synchronize[animal][j] == true)
				{
					break;
				}	
				continue;
			}

			if(j == prio.threads_per_group[animal]-1)
			{
				if(verbose == true)
					printf("-------------------------------- SCHEDULER GOES SLEEP\n");
					
				while(true)
				{
						if(checkIfEmpty(animal) == true)
						{
							break;	
						}
						continue;
				}
				end = clock();
				double diff = ((double) end - begin)/CLOCKS_PER_SEC;
				waiting_times_group[animal] += diff;
								
				if(verbose == true)
				printf("-------------------------------- SCHEDULER WAKES UP\n");
			}
			
			
		}
		//increment group priorities
		calcGroupPriorities(animal);	
		
		++cnt;
		
}
	
	printf("---------------------------------- RESULT ----------------------------------\n");
	if(verbose == true)
	{
		printf("Animal Group %d is done %d\n", 0, groupIsDone(0));
		printf("Animal Group %d is done %d\n", 1, groupIsDone(1));
		printf("Animal Group %d is done %d\n", 2, groupIsDone(2));
	}
	
	printStatistics();
}
