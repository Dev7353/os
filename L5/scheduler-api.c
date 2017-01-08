#include "scheduler-api.h"
#include <stdio.h>
#include <time.h>
int nextBowle(char* status)
{
	int i = 0;
	for(;status[i] != '\0'; ++i){}
	
	return i;
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
