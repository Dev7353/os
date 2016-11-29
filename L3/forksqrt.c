#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include "ini.h"
#include <sys/wait.h>
#include <assert.h>

#define DEFAULTCONFIG "forksqrt.cfg"
#define PIPESIDES 2
#define READBUFFERSIZE 1000
#define PARSESIZE 3
#define READSIDE 0
#define WRITESIDE 1
#define SEPERATORSIZE 4

typedef struct
{
    char *start;
    char *loops;
    char *tolerance;
    char *numbers;
} configuration;

static int
handler (void *sqrt2, const char *section, const char *name,
         const char *value)
{
    configuration *pconfig = (configuration *) sqrt2;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH ("sqrt2", "start"))
    {
        pconfig->start = strdup (value);
    }
    else if (MATCH ("sqrt2", "loops"))
    {
        pconfig->loops = strdup (value);
    }
    else if (MATCH ("sqrt2", "tolerance"))
    {
        pconfig->tolerance = strdup (value);
    }
    else if (MATCH ("sqrt2", "numbers"))
    {
        pconfig->numbers = strdup (value);
    }
    else
    {
        return 0;         /* unknown section/name, error */
    }
    return 1;
}

int
main (int argc, char *argv[])
{
    configuration config;
    pid_t pid;
    char readbuffer[READBUFFERSIZE] = "";
    char *buffer;
    int pipefdarray[4][2];
    int errno = 0;
    int status;
    char *configfile = NULL;
    int flag = 0;
    int c;
    int i, fragmentSize;
    FILE *file;
    configfile = DEFAULTCONFIG;
    char buf[1000];
    int pipeNumber = 1;

    while ((c = getopt (argc, argv, "hdc:f:ops")) != -1)
    {
        switch (c)
        {
        case 'h':
            printf ("-h\t\tShow help\n-d\t\tPrints debugging output to the console\n-c [FILENAME]\tReads configuration out of config file with name FILENAME. \n\t\tIf no FILENAME is provided, the default config file is 'forksqrt.cfg'\n");
            return 0;
        case 'd':
            flag = 1;
            break;
        case 'c':
            if (optarg == NULL)
                break;
            else
                configfile = optarg;
            break;
        case 'f':
            file = fopen(optarg, "r");
            if (file) {
                while (fgets(buf, 1000, file) != NULL)
                    printf("%s", buf);
                fclose(file);
            }
            break;
        case 'o':
            stdout = fopen("log.txt", "a");
            break;
        case 'p':
            pipeNumber = 4;
            break;
        case 's':
            fragmentSize = atoi(optarg);
        }

    }

    if (ini_parse (configfile, handler, &config) < 0)
    {
        perror ("Can't load config'\n");
        return 1;
    }

    for (i = 0; i < pipeNumber; ++i) {
        if (pipe(pipefdarray[i]) != 0)
        {
            printf("Failed Syscall: %d\n", errno);
            perror("Pipe not successful.\n");
        }
        pid = fork ();
        if (pid < 0)
        {
            printf("Failed Syscall: %d\n", errno);
            perror("Fork not successful. \n");
        }
        if (pid == 0) //Child process
        {
            // Parse file descriptors as strings to be able to send them as parameters
            char fd0[PARSESIZE], fd1[PARSESIZE];
            snprintf (fd0, PARSESIZE, "%d", pipefdarray[i][0]);
            snprintf (fd1, PARSESIZE, "%d", pipefdarray[i + 1][1]);
            char *python[] = { "python3", "forksqrt.py", fd0, fd1, NULL };
            execvp ("python3", python);
        }
    }

    // Close unused pipe sides
    if (close (pipefdarray[0][READSIDE]) != 0)
    {
        printf("Failed Syscall: %d\n", errno);
        perror("Close not successful. Close writeToChild.\n");
    }
    if (close (pipefdarray[1][WRITESIDE]) != 0)
    {
        printf("Failed Syscall: %d\n", errno);
        perror("Close not successful. Close readFromChild.\n");
    }
    if (pipeNumber == 4) {
        if (close (pipefdarray[2][READSIDE]) != 0)
        {
            printf("Failed Syscall: %d\n", errno);
            perror("Close not successful. Close writeToChild.\n");
        }
        if (close (pipefdarray[3][WRITESIDE]) != 0)
        {
            printf("Failed Syscall: %d\n", errno);
            perror("Close not successful. Close readFromChild.\n");
        }
    }
    // Build a string with all the parameters to pipe through to child
    buffer = (char *) malloc (sizeof (config) + SEPERATORSIZE);
    assert(buffer != NULL);
    strcpy (buffer, config.start);
    strcat (buffer, "|");
    strcat (buffer, config.loops);
    strcat (buffer, "|");
    strcat (buffer, config.tolerance);
    strcat (buffer, "|");
    strcat (buffer, config.numbers);

    // Send correct debug parameter depending on set flag
    if (flag == 1)
    {
        strcat (buffer, "|");
        strcat (buffer, "True");
    }
    else
    {
        strcat (buffer, "|");
        strcat (buffer, "False");
    }
    // Write the data to child and read the result from it
    if (write (pipefdarray[0][WRITESIDE], buffer, strlen (buffer)) == -1)
    {
        printf("Failed Syscall: %d\n", errno);
        perror("Write not successful.\n");
    }
    if (read (pipefdarray[1][READSIDE], readbuffer, sizeof (readbuffer)) == -1)
    {
        printf("Failed Syscall: %d\n", errno);
        perror("Read not successful.\n");
    }
    if (pipeNumber == 4) {
        if (write (pipefdarray[2][WRITESIDE], buffer, strlen (buffer)) == -1)
        {
            printf("Failed Syscall: %d\n", errno);
            perror("Write not successful.\n");
        }
        if (read (pipefdarray[3][READSIDE], readbuffer, sizeof (readbuffer)) == -1)
        {
            printf("Failed Syscall: %d\n", errno);
            perror("Read not successful.\n");
        }
    }
    // Print the results
    printf ("Results: %s\n", readbuffer);
    for (i = 0; i < pipeNumber; ++i) {
        wait (&status);
    }
    if (WIFEXITED (status))
    {
        if (status != 0)
        {
            printf
            ("Wait not successful. Exitcode: %d. Failed Syscall: %d.\n", WEXITSTATUS (status), errno);
        }
    }

    // Free all allocated memory
    free (buffer);
    free (config.start);
    free (config.loops);
    free (config.tolerance);
    free (config.numbers);

    return 0;
}
