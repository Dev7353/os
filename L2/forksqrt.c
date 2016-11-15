# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <string.h>
# include <unistd.h>
# include "inih/ini.h"
# include <sys/wait.h>

typedef struct
{
    char* start;
    char* loops;
    char* tolerance;
    char* numbers; 
} configuration;

static int handler(void* sqrt2, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)sqrt2;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("sqrt2", "start")) {
        pconfig->start = strndup(value, 1);
    } else if (MATCH("sqrt2", "loops")) {
        pconfig->loops = strdup(value);
    } else if (MATCH("sqrt2", "tolerance")) {
        pconfig->tolerance = strdup(value);
    } else if (MATCH("sqrt2", "numbers")) {
    	pconfig->numbers = strdup(value);
 
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

int main(void)
{
	configuration config;
	pid_t pid;
    int writeToChild[2]; 
    int readFromChild[2];
    //char numbers[];
    //char readbuffer[80];

    if (ini_parse("forksqrt.cfg", handler, &config) < 0) {
        printf("Can't load config'\n");
        return 1;
    }
    printf("Config loaded from 'forksqrt.cfg': start=%s, loops=%s, tolerance=%s, numbers=%s",
        config.start, config.loops, config.tolerance, config.numbers);
    printf("\n");
    
    pipe(writeToChild);
    pipe(readFromChild);
    pid = fork();
    if(pid == 0)
    {
        //printf("%d\n", writeToChild[1]);
        //close(writeToChild[1]);
        //close(readFromChild[0]);
        //read(writeToChild[0], readbuffer, sizeof(readbuffer));
        //printf("String: %s\n", readbuffer);
        //char *argument = "./forksqrt.py";
        char fd0[20], fd1[20];
        snprintf(fd0, 20, "%d", writeToChild[0]);
        snprintf(fd1, 20, "%d", readFromChild[1]);
        printf("%s\n", fd0);
        char *python[] = {"python3", "forksqrt.py", fd0, fd1, NULL};
        execvp("python3", python);
    }
    //int status;
    close(writeToChild[0]);
    close(readFromChild[1]);
    
    char *buffer = (char*) malloc(sizeof(config) + 4);
    
    strcpy(buffer, config.start);
    strcat(buffer, "|");
    strcat(buffer, config.loops);
    strcat(buffer, "|");
    strcat(buffer, config.tolerance);
    strcat(buffer, "|");
    strcat(buffer, config.numbers);
    
    printf("BUFFER %s\n", buffer);
    
    write(writeToChild[1], buffer, sizeof(config)+4);
    //read(readFromChild[0], readbuffer, sizeof(readbuffer));
    //printf("Readbuffer: %s\n", readbuffer);
    wait(NULL);
    return 0;
}
