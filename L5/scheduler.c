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

/*define function prototypes incl thread functions*/
void scheduler(void* arg);
void eat(void* arg);
pthread_cond_t* nextAnimal();
int nextGroup();
boolean animalsDone(int animal);
boolean workIsDone();
/*define global variables*/
food_area area;
int** threadDone;
volatile int isReady[GROUPS];
pthread_cond_t* cond_cats;
pthread_cond_t* cond_dogs; 
pthread_cond_t* cond_mice;
pthread_cond_t cond_scheduler;
pthread_cond_t** cond_container; //wrapper for animal cvs
typedef struct 
{
	pthread_cond_t** container; //wrapper for cvs for eah animal
	int** priority; // prioritiy for each animal 
	int* group_priority; // priority for each animal group
	int* threads_per_group;
	
}prio_queue_t;

prio_queue_t prio;
pthread_mutex_t mutex;
boolean verbose = false;

int main(int argc, char* argv[])
{
	
	char* filename = NULL;
	//Default amount of animals
	int cn = 6;
	int dn = 4;
	int mn = 2;
	
	//Default amount threads per animal group
	prio.threads_per_group = (int*) malloc(GROUPS * sizeof(int));
	prio.threads_per_group[0] = cn;
	prio.threads_per_group[1] = dn;
	prio.threads_per_group[2] = mn;
	
	prio.group_priority = (int*) calloc(GROUPS, sizeof(int));
	
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
	
	cond_container = (pthread_cond_t**) malloc(GROUPS * sizeof(pthread_cond_t*));
	cond_container[0] = cond_cats;
	cond_container[1] = cond_dogs;
	cond_container[2] = cond_mice;
	
	prio.container = cond_container;
	prio.priority = (int**) malloc(3 * sizeof(int));
	prio.priority[0] = (int*) calloc(cn, sizeof(int));
	prio.priority[1] = (int*) calloc(dn, sizeof(int));
	prio.priority[2] = (int*) calloc(mn, sizeof(int));
	
	animal_t* cat_args = (animal_t*) malloc(cn * sizeof(animal_t));
	animal_t* dog_args = (animal_t*) malloc(dn * sizeof(animal_t));
	animal_t* mouse_args = (animal_t*) malloc(mn * sizeof(animal_t));
	
	
	area.bowles = num_dishes;
	area.num_eaten = 0;
	area.status = (char*) malloc(sizeof(char) * num_dishes);
	for(int i = 0; i < num_dishes; ++i)
		area.status[i] = '-';

	threadDone = (int**) malloc(GROUPS * sizeof(int*));
	for(int i = 0; i < GROUPS; ++i)
	{
			if(i == 0)
				threadDone[i] = (int*) calloc(cn, sizeof(int));
			else if(i == 1)
				threadDone[i] = (int*) calloc(dn, sizeof(int));
			else
			threadDone[i] = (int*) calloc(mn, sizeof(int));
	}
	
	time_t sec;
	time(&sec);
	srandom ((unsigned int) sec);
	
	/*start threads*/
	assert(pthread_cond_init(&cond_scheduler, NULL) == 0);
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
{	
	int animal = 0;
	animal_t param = *((animal_t*)arg);
	while(param.num_eat > 0)
	{
		/*animal gets hungry*/
		sleep(param.satisfied_time);
		if(verbose == true)
			printf("%s %d is hungry\n", param.animal_type, param.id);
		
		if(strcmp(param.animal_type, CAT) == 0)
		{
			isReady[0]++;
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_cats[param.id], &mutex);
			pthread_mutex_unlock(&mutex);
		}
		else if(strcmp(param.animal_type, DOG) == 0)
		{
			animal = 1;
			isReady[1]++;
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_dogs[param.id], &mutex);
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			animal = 2;
			isReady[2]++;
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_mice[param.id], &mutex);
			pthread_mutex_unlock(&mutex);
		}
		
		if(param.num_eat == 0)
			continue;
		
		int my_dish = nextBowle(area.status);
		//area.status[my_dish] = param.animal_type[0];
		time_t t = time(NULL);
		struct tm tm = *localtime(&t);
		 printf("\t[%d-%d-%d %d:%d:%d] [%s] %s %d started eating from dish %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, area.status, param.animal_type, param.id, my_dish);
		--param.num_eat;
		++area.num_eaten;
		sleep(param.eating_time);
		pthread_cond_signal(&cond_scheduler); //send signal after eating time
		
		--area.num_eaten;
		t = time(NULL);
		tm = *localtime(&t);
		printf("\t[%d-%d-%d %d:%d:%d] [%s] %s %d finished eating from dish %d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, area.status, param.animal_type, param.id, my_dish);
			
	}
	
	threadDone[animal][param.id] = true;
	printf("%d %s is done\n", param.id, param.animal_type);
	
}

void scheduler(void* arg)
{
	int cnt = 0;
	while(true)
	{
		int animal = nextGroup();
		
		if(verbose == true)
				printf("--------------------------------> SCHEDULER DEBUG INFO: GROUP: %d PRIORITY: %d\n", animal, prio.group_priority[animal]);

		while(true)
		{
			if(isReady[animal] < area.bowles)
			{
					continue;
			}
			break;
		}
		
		if(verbose == true)
			printf("-------------------------------- Scheduler starts round number %d\n", cnt);
			
		for(int j = 0; j < area.bowles; ++j)
		{
			pthread_cond_signal(nextAnimal(animal));
			while(true)
			{	
				if(area.num_eaten == 0)
				{
					continue;
				}
				break;
			}
			if(verbose == true)
				printf("-------------------------------- SCHEDULER GOES SLEEP\n");
			pthread_mutex_lock(&mutex);
			pthread_cond_wait(&cond_scheduler, &mutex);
			pthread_mutex_unlock(&mutex);
			if(verbose == true)
				printf("-------------------------------- SCHEDULER WAKES UP\n");
		}

		
		++cnt;
		
		if(workIsDone() == true)
			break;
	}
}

pthread_cond_t* nextAnimal(int animal) 
{
	int min = prio.priority[animal][0];
	int index = 0;
	for(int i = 0; i < prio.threads_per_group[animal]; ++i)
	{
		if(prio.priority[animal][i] < min)
		{
			min = prio.priority[animal][i];
			index = i;
		}
	}
	
	if(verbose == true)
	{
		if(animal == 0)
			printf(">>>> the next animal is cat %d\n", index);
		if(animal == 1)
			printf(">>>> the next animal is dog %d\n", index);	
		if(animal == 2)
			printf(">>>> the next animal is mouse %d\n", index);
	}
	
	prio.priority[animal][index]++;
	//printf("DEBUG %d\n", index);
	return &prio.container[animal][index];

}

int nextGroup()
{
	int min_index = 0;
	int min_prio = prio.group_priority[0];
	
	for(int i = 0; i < GROUPS; ++i)
	{
		if(animalsDone(i) == false)
		{
			
		
			if(prio.group_priority[i] < min_prio)
			{
				min_index = i;
				break;
			}
		}
	}
	
	prio.group_priority[min_index]++; 
	
	return min_index;
	
}

boolean animalsDone(int animal)
{
	for(int i = 0; i < prio.threads_per_group[animal]; ++i)
	{
		if(threadDone[animal][i] == false)
			return false;
	}
	return true;
}

boolean workIsDone()
{
		for(int i = 0; i < GROUPS; ++i)
		{
				if(animalsDone(i) == false)
					return false;
		}
		
		return true;
}
