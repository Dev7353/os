# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <string.h>
# include <unistd.h>
# include "inih/ini.h"

typedef struct
{
    int start;
    int loops;
    float tolerance;
    int numbers[3];
} configuration;

static int handler(void* sqrt2, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)sqrt2;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("sqrt2", "start")) {
        pconfig->start = atoi(value);
    } else if (MATCH("sqrt2", "loops")) {
        pconfig->loops = atoi(value);
    } else if (MATCH("sqrt2", "tolerance")) {
        pconfig->tolerance = atof(value);
    } else if (MATCH("sqrt2", "numbers")) {
    	sscanf(value, "%d,%d,%d", & pconfig->numbers[0], & pconfig->numbers[1], & pconfig->numbers[2]);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

int main(void)
{
	configuration config;
	pid_t pid;
	int i;

    if (ini_parse("forksqrt.cfg", handler, &config) < 0) {
        printf("Can't load config'\n");
        return 1;
    }
    printf("Config loaded from 'forksqrt.cfg': start=%d, loops=%d, tolerance=%.14f, numbers=",
        config.start, config.loops, config.tolerance);
    for(i = 0; i<3; i++) {
    	printf("%d,", config.numbers[i]);
    }
    printf("\n");
    
    pid = fork();
    if(pid == 0)
    {
        execlp("python3", "python3", "forksqrt.py", "", (char*) NULL);
    }
    return 0;
}
