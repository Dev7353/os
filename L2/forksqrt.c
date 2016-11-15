# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <string.h>
# include <unistd.h>
# include "inih/ini.h"
# include <sys/wait.h>

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
    int writeToChild[2]; 
    int readFromChild[2];
    char string[] = "Hello, world!\n";
    char readbuffer[80];

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
    
    pipe(writeToChild);
    pipe(readFromChild);
    pid = fork();
    if(pid == 0)
    {
        //printf("%d\n", writeToChild[1]);
        close(writeToChild[1]);
        close(readFromChild[0]);
        read(writeToChild[0], readbuffer, sizeof(readbuffer));
        //printf("String: %s\n", readbuffer);
        //char *argument = "./forksqrt.py";
        char fd0[20], fd1[20];
        snprintf(fd0, 20, "%d", writeToChild[0]);
        snprintf(fd1, 20, "%d", readFromChild[1]);
        char *python[] = {"python3", "forksqrt.py", fd0, fd1, NULL};
        execvp("python3", python);
    }
    //int status;
    close(writeToChild[0]);
    close(readFromChild[1]);
    write(writeToChild[1], string, (strlen(string)+1));
    //read(readFromChild[0], readbuffer, sizeof(readbuffer));
    //printf("Readbuffer: %s\n", readbuffer);
    wait(NULL);
    return 0;
}
