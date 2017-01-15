#include "time-api.h"
#include "scheduler-api.h"

double getMin(int animal, int animal_threads)
{
	double min = waiting_times[animal][0];
	for(int i = 0; i < animal_threads* area.eating_times_per_group[animal]; ++i)
	{
		if(waiting_times[animal][i] <= min)
			min = waiting_times[animal][i];
	}
	
	return min;	
}

double getAvg(int animal, int animal_threads)
{
	double sum = 0;
	for(int i = 0; i < animal_threads* area.eating_times_per_group[animal]; ++i)
	{
		sum += waiting_times[animal][i];
	}
	
	return sum/(animal_threads*area.eating_times_per_group[animal]);
}

double getMax(int animal, int animal_threads)
{
	double max = waiting_times[animal][0];
	for(int i = 0; i < animal_threads * area.eating_times_per_group[animal]; ++i)
	{
		if(waiting_times[animal][i] >= max)
			max = waiting_times[animal][i];
	}
	
	return max;
}

int nextTimeSlot(int animal)
{
	int index = 0;
	
	for(int i = 0; i < prio.threads_per_group[animal]*area.eating_times_per_group[animal];++i)
		if(waiting_times[animal][i] == 0)
		{
			index = i;
			break;	
		}
	return index;
}
