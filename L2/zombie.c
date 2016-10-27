# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/wait.h>

int main(void)
{
    pid_t pid;
    pid = fork();
    if(pid == 0)
    {
        exit(0);
    }
    
    
    system("ps -eo pid,ppid,stat,command > ids");
    system("cat ids | grep 'zombie'");
    
    return 0;
}
