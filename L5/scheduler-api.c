#include "scheduler-api.h"
#include "time-api.h"
#include <stdlib.h>
#include <assert.h>

int nextBowle()
{
	int idx = -1;
	for(int i = 0; i < area.bowles; ++i)
	{
		if(area.status[i] == '-')
		{
			idx = i;
			break;
		}
	}
	
	return idx;
}

void printHelp()
{
	printf("SCHEDULER HELP:\n");
	printf("\t--verbose \t debugging prints\n");
	printf("\t -f --file \targ: file to flush data into\n");
	printf("\t -h --help \tprint arguments list\n");
	printf("\t -a --cn \targ: number of cats\n");
	printf("\t -b --dn \targ: number of dogs\n");
	printf("\t -c --mn \targ: number of mice\n");
	
	printf("\t -d --ct \targ: time a cat is satisfied\n");
	printf("\t -g --dt \targ: time a dog is satisfied\n");
	printf("\t -i --mt \targ: time a mouse is satisfied\n");
	
	printf("\t -j --ce \targ: how many times a cat wants to eat\n");
	printf("\t -k --de \targ: how many times a dog wants to eat\n");
	printf("\t -l --me \targ: how many times a mouse wants to eat\n");
	
	printf("\t -m --e \targ: eating time interval lower boundary\n");
	printf("\t -n --E \targ: eating time interval upper boundary\n");
	
	printf("\t -o --dish \targ: Number of Food-Dishes\n");
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
			printf("%s>>>> the next animal is cat %d%s\n",ANSI_COLOR_GREEN, index, ANSI_COLOR_RESET);
		if(animal == 1)
			printf("%s>>>> the next animal is dog %d%s\n", ANSI_COLOR_GREEN, index, ANSI_COLOR_RESET);	
		if(animal == 2)
			printf("%s>>>> the next animal is mouse %d%s\n", ANSI_COLOR_GREEN, index, ANSI_COLOR_RESET);
	}
	
	prio.priority[animal][index]++;
	return &prio.container[animal][index];

}

int nextGroup()
{
	int min_index = -1;
	
	//get the next AVAILABLE minima grouppriority
	int min_prio = 0;
	for(int i = 0; i < GROUPS; ++i)
	{
		if(groupIsDone(i) ==  false)
		{
			min_prio = prio.group_priority[i];
			break;
		}
	} 
	
	for(int i = 0; i < GROUPS; ++i)
	{
		if(groupIsDone(i) == false)
		{	
			if(verbose == true)
				printf("%s[GROUP OF %d HAS THE PRIORITY %d]%s\n",ANSI_COLOR_GREEN, i, prio.group_priority[i], ANSI_COLOR_RESET);
			if(prio.group_priority[i] <= min_prio)
			{
				min_index = i;
				min_prio = prio.group_priority[i];
			}
		}
	}
	
	if(verbose == true)
		printf("\t%s[NEXT GROUP IS %d]%s\n",ANSI_COLOR_GREEN, min_index, ANSI_COLOR_RESET);
	return min_index;
	
}

boolean groupIsDone(int animal)
{
	boolean erg = true;
	for(int i = 0; i < prio.threads_per_group[animal]; ++i)
	{
		erg *= threadDone[animal][i]; 
	}
	return erg;
}

void calcGroupPriorities(int current_group)
{

	for(int i = 0; i < GROUPS; ++i)
	{
		if(i == current_group || groupIsDone(i) == true)
			continue;
		else 
		{
			prio.group_priority[i] --;
		}
	}
	
	prio.group_priority[current_group] ++;
}

boolean checkIfEmpty(int animal)
{
	int ctr = 0;
	int should =  prio.threads_per_group[animal];
	for(int i = 0; i < prio.threads_per_group[animal];++i)
	{
		if(synchronize[animal][i] == false)
			++ctr;
		//case for just 1 bowle
		else if(area.bowles == 1 && synchronize[animal][i] == -1) 
			++ctr;
	} 
	if(ctr == should)
		return true;
	
	return false;
}

void initializeGlobals()
{
	threadDone = (int**) malloc(GROUPS * sizeof(int*));
	assert(threadDone != NULL);
	
	synchronize = (int**) malloc(GROUPS * sizeof(int*));
	assert(synchronize != NULL);
	
	waiting_times = (double**) malloc(GROUPS * sizeof(double*));
	assert(waiting_times != NULL);
	
	waiting_times_group = (double*) calloc(GROUPS, sizeof(double));
	assert(waiting_times_group != NULL);
	for(int i = 0; i < GROUPS; ++i)
	{
		threadDone[i] = (int*) calloc(prio.threads_per_group[i], sizeof(int)); 
		assert(threadDone[i] != NULL);
		synchronize[i] = (int*) calloc(prio.threads_per_group[i], sizeof(int));
		assert(synchronize[i] != NULL);
		waiting_times[i] = (double*) calloc(prio.threads_per_group[i]*area.eating_times_per_group[i], sizeof(double));
		assert(waiting_times[i] != NULL); 
	} 
	for(int i = 0; i < GROUPS; ++i)
	{

		for(int j = 0; j < prio.threads_per_group[i]; ++j)
			synchronize[i][j] = -1; 
	}
}

void printStatistics()
{
	for(int i = 0; i < GROUPS; ++i)
	{
		if(i == 0)
			printf("%s:\n", CAT);
		else if(i == 1)
			printf("%s:\n", DOG);
		else
			printf("%s:\n", MOUSE);
			
		printf("\tMin: %f sec\n", getMin(i, prio.threads_per_group[i]));
		printf("\tMax: %f sec\n", getMax(i, prio.threads_per_group[i]));
		printf("\tAvg: %f sec\n", getAvg(i, prio.threads_per_group[i]));
		
		if(verbose == true || file == true)
		{
			int cat_ctr = 0;
			int dog_ctr = 0;
			int mouse_ctr = 0;
			for(int j = 0; j < area.eating_times_per_group[i] * prio.threads_per_group[i]; j++)
			{
				
				double x = j;
				double y = waiting_times[i][j];
				
				if(verbose == true)
					printf("%f, %f\n", x, y);
								
				fp = fopen(filename,"a");
				if(i == 0 && cat_ctr == 0)
				{	
					fprintf(fp, "%s\n", "cats"); 
					cat_ctr++;
				}
				else if(i == 1 && dog_ctr == 0)
				{
					fprintf(fp, "%s\n", "dogs"); 
					dog_ctr++;
				}
				else if (i == 2 && mouse_ctr == 0)
				{
					fprintf(fp, "%s\n", "mice"); 
					mouse_ctr++;
				}
				fprintf(fp, "%f,%f\n", x, y);
				fclose(fp);
			}	
			
		}
	
	}
			if(file == true)
			{
				fp = fopen(filename,"a");
				fprintf(fp, "%s", "---\n");
				for(int i = 0; i < GROUPS; ++i)
				{
					if(i == 0)
						fprintf(fp, "%s:\n", CAT);
					else if(i == 1)
						fprintf(fp, "%s:\n", DOG);
					else
						fprintf(fp, "%s:\n", MOUSE);
						
					fprintf(fp, "\tMin: %f sec\n", getMin(i, prio.threads_per_group[i]));
					fprintf(fp, "\tMax: %f sec\n", getMax(i, prio.threads_per_group[i]));
					fprintf(fp, "\tAvg: %f sec\n", getAvg(i, prio.threads_per_group[i]));
				}
				fclose(fp);
			}
}

void threadIsDone(int animal)
{
	for(int i = 0; i < prio.threads_per_group[animal]; ++i)
		threadDone[animal][i] = true;
}

void freeGlobals()
{	
	for(int i = 0; i < GROUPS; ++i)
	{
		free(threadDone[i]);
		free(synchronize[i]);
		free(waiting_times[i]);
		free(prio.container[i]);
		free(prio.priority[i]);

	}
	
	free(threadDone);
	free(synchronize);
	free(waiting_times);
	free(waiting_times_group);
	free(prio.container);
	free(prio.threads_per_group);
	free(prio.group_priority);
	free(prio.priority);
	 
}
